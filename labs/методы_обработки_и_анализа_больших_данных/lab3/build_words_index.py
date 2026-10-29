import json
import math
import os
import sys
import time
from collections import defaultdict
from multiprocessing import Pool

from timer import Time

base_dir = os.path.dirname(os.path.abspath(__file__))
input_path = os.path.join(base_dir, 'data', 'wiki titles no special.txt')
output_path = os.path.join(base_dir, 'data', 'words in titles1.json')

def process_chunk(args):
    start_index, chunk = args
    local_words = defaultdict(list)

    for i, name in enumerate(chunk):
        article_idx = start_index + i
        for word in name.split(' '):
            if word:
                local_words[word].append(article_idx)

    return dict(local_words)

def to_chunks(lst, n):
    chunks = list()

    chunk_size = math.ceil(len(lst) / n)

    for i in range(n):
        start = i * chunk_size
        end = min((i + 1) * chunk_size, len(lst))
        if start < len(lst):
            chunks.append((start, lst[start:end]))

    return chunks

if __name__ == '__main__':
    print(f'Чтение файла: {input_path}')
    t0 = time.time()
    with open(input_path, 'r', encoding='utf-8') as f:
        article_names = [line.strip() for line in f]

    print(f'Загружено {len(article_names):,} заголовков за {time.time() - t0:.2f} сек.')

    num_processes = os.cpu_count() or 4
    print(f'Запуск параллельной обработки на {num_processes} процессах...')

    # Разбиваем список на num_processes блоков со смещениями
    chunks_args = to_chunks(article_names, num_processes)

    with Time('Параллельная обработка блоков'):
        with Pool(processes=num_processes) as pool:
            results = pool.map(process_chunk, chunks_args)

    print('Объединение результатов от процессов...')
    t0 = time.time()
    words = defaultdict(list)
    for res in results:
        for word, indices in res.items():
            words[word].extend(indices)
    print(f'Объединение завершено за {time.time() - t0:.2f} сек. Всего уникальных слов: {len(words):,}')

    print(f'Сохранение результата в {output_path}...')
    t0 = time.time()
    with open(output_path, 'w', encoding='utf-8') as f:
        json.dump(words, f)
    print(f'Файл успешно сохранён за {time.time() - t0:.2f} сек.')