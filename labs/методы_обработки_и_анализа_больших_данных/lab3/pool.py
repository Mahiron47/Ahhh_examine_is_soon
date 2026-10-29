import math
from functools import reduce
from multiprocessing import Pool
import itertools as it
import os
import sys
import json

from timer import Time
from distance.levenshtein import levenshtein


# путь к файлу с названиями статей Википедии без специальных символов
article_names_path = os.path.join(os.path.dirname(__file__), 'data', 'wiki titles no special.txt')
words_path = os.path.join(os.path.dirname(__file__), 'data', 'words in titles.json')

def print_results(words_list, words, article_names, max_articles_per_word=5):
    for word, distance in words_list:
        word_article_indexes = words.get(word, [])
        print(f"Match: '{word}' with distance {distance}:")

        for index in word_article_indexes[:max_articles_per_word]:
            print(f'    Article: {article_names[index]}')

        if len(word_article_indexes) > max_articles_per_word:
            print(f'    ... and {len(word_article_indexes) - max_articles_per_word} more articles.')
        print()


# сравнение запроса (word) с кандидатом (ref)
def mapper(word, ref):
    if abs(len(word) - len(ref)) > 2:
        return ref, 10

    set_word = set(word.lower())
    set_ref = set(ref.lower())
    if (len(set_word - set_ref) > 2) or (len(set_ref - set_word) > 2):
        return ref, 10

    return ref, levenshtein(word, ref, lowercase=True)


# выбор 10 лучших результатов (с наименьшим редакционным расстоянием)
def reducer(current_ref_list, new_ref):
    if not isinstance(current_ref_list, list):
        current_ref_list = [current_ref_list]

    if isinstance(new_ref, list):
        current_ref_list.extend(new_ref)
    else:
        current_ref_list.append(new_ref)

    current_ref_list.sort(key=lambda x: x[1])
    return current_ref_list[:10]


# разбиение набора данных на N блоков
def chunkify(refs, N):
    step = int(math.ceil(len(refs) / N))
    inds = list(range(0, len(refs) + step, step))
    for i1, i2 in zip(inds, inds[1:]):
        yield refs[i1:i2]


# обработка одного блока
def chunk_processer(word, refs):
    mapped = map(mapper, it.repeat(word), refs)
    reduced = reduce(reducer, mapped)
    return reduced


# основная часть скрипта
# Pool должен использоваться здесь
if __name__ == '__main__':
    # читаем список слов, которые встречаются в названиях статей Википедии
    with open(words_path, 'r', encoding='utf-8') as f:
        words = json.load(f)

    # получаем список названий статей без пробельных символов в начале и конце
    with open(article_names_path, 'r', encoding='utf-8') as f:
        article_names = [line.strip() for line in f]

    # задаем слово для поиска или читаем его из командной строки (если передан аргумент)
    word = 'hi'
    if len(sys.argv) > 1:
        word = sys.argv[1]

    print(f'Request: {word}\n')

    words_list = list(words.keys())

    # ищем ближайшее совпадение последовательно
    # при последующих запусках эту часть лучше закомментировать, чтобы не тратить время
    with Time('Последовательные вычисления'):
        mapped = map(mapper, it.repeat(word), words_list)
        reduced = reduce(reducer, mapped)
    print_results(reduced, words, article_names)

    # Time считает время выполнения следующего блока и выводит его с комментарием, переданным в качестве аргумента
    N = os.cpu_count()  # количество процессов, которое будет использоваться в пуле

    with Time('Параллельные вычисления'):
        # ищем ближайшее совпадение параллельно в N процессах
        with Pool(processes=N) as pool:
            word_chunks = list(chunkify(words_list, N))
            mapped = pool.starmap(chunk_processer, zip(it.repeat(word), word_chunks))
            reduced = reduce(reducer, mapped)
    print_results(reduced, words, article_names)