# Архитектура:
### Отчеты:
macbook - пару слов про macbook на m4
REPORT - основной отчет

каждый из файлов кода принадлежит одному из 6 подмножеств файлов множеств:
##### kernel
Список версий:
+ mul-0 - наивная
+ mul-1 - квадратный тайлинг + "нативная" оптимизация с банками памяти
+ mul-2-short - квадратный тайлинг + квадратное разбиение на thread work
+ mul-2-access - тайлинг + пряоугольное разбиение на thread work + равномерный, но не оптимизированный доступ к глобальной памяти в момент чтения global -> local
+ mul-2 - (полный список оптимизаций) v-2-access + выровненный по горизонтали доступ к глобальной памяти + векторные типы + оптимизация для банковых конфликтов.
##### library - перемножение матриц
.h - multiply-matrices, lib-utils, data-structures
.src - gpu-calc, cpu-calc, gpu-utils
##### тесты, исследования
.h - all-tests, test-utils
.src:
+ test-gpu-perf-artificial-factor
+ test-optimal-tile-size
+ test-simple
+ test-stress-v2
+ test-transpose-cpu-correctness
+ test-v2-short
##### программа из задания
.h - utils
.src - main, utils
##### library - печатание таблиц в формате csv
.h - csv-printer
.src - csv-printer
##### main
main.c
