# Minecraft 1.20.1 Source Code

Этот каталог содержит полный исходный код Minecraft 1.20.1 с mappings Yarn, извлечённый из проекта CubicChunks.

## Статистика
- **Всего файлов:** 5,058
- **Java файлов:** 4,786
- **Версия:** 1.20.1
- **Mappings:** net.fabricmc.yarn.1_20_1.1.20.1+build.10-v2

## Основные пакеты

### `net.minecraft`
Корневой пакет Minecraft со всеми основными компонентами:

- **`client/`** - Клиентский код
  - `gui/` - GUI интерфейсы, включая экраны создания мира
  - `render/` - Рендеринг
  - `network/` - Сетевое взаимодействие клиента
  - `world/` - Клиентский мир

- **`server/`** - Серверный код
  - `MinecraftServer.java` - Основной класс сервера
  - `command/` - Команды сервера
  - `network/` - Сетевое взаимодействие сервера
  - `world/` - Серверный мир

- **`world/`** - Игровой мир
  - `gen/` - Генерация мира, включая `WorldPresets.java`
  - `chunk/` - Чанки
  - `biome/` - Биомы
  - `dimension/` - Измерения

- **`block/`** - Блоки
- **`entity/`** - Сущности
- **`item/`** - Предметы
- **`util/`** - Утилиты

## Ключевые файлы для CubicChunks

### GUI создания мира:
- `client/gui/screen/world/CreateWorldScreen.java`
- `client/gui/screen/world/WorldCreator.java`
- `world/gen/WorldPresets.java`

### Генерация мира:
- `world/gen/GeneratorOptions.java`
- `world/gen/chunk/`
- `world/chunk/`

### Основные классы:
- `MinecraftVersion.java`
- `SharedConstants.java`
- `client/MinecraftClient.java`
- `server/MinecraftServer.java`