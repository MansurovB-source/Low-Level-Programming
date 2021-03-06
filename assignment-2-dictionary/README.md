---

# Темы практики

- Цикл компиляции
- Линковщик; как собрать программу из нескольких модулей
- GNU Make и Make-файлы
- Препроцессор и его основные директивы

# Цикл компиляции

Исходный код преобразуется в набор машинных инструкций, которые можно выполнить,
в несколько больших этапов (каждый из них может состоять из многих меньших этапов):

1. Препроцессинг — преобразование текстового файла на ассемблере с директивами
   макропроцессора в текстовый файл на ассемблере уже без этих директив. Этим
   занимается **препроцессор** — отдельная программа или часть компилятора.
2. Трансляция — преобразование текстового файла на ассемблере в объектный файл.
   Этим занимается сам **компилятор** Объектный файл содержит инструкции, но
   адреса в них не проставлены. Пояснения далее.
3. Компоновка (линковка) — специальная программа **компоновщик** собирает
   исполняемый файл с инструкциями из всех объектных файлов.

Если программа содержит несколько файлов, то каждый из них проходит стадии (1) и
(2) отдельно. Затем все объектные файлы мы передаем компоновщику. Пример для
трёх ассемблерных файлов `a.asm` `b.asm` `c.asm`

```sh
> nasm -f elf64 -o a.o a.asm
> nasm -f elf64 -o b.o b.asm
> nasm -f elf64 -o c.o c.asm
> ld -o program a.o b.o c.o
```

## Глобальные символы

Чтобы файлы видели код друг друга, необходимо описать их интерфейсы. 
Пусть файл `a.asm` содержит функции `f`, `g` и `h`; мы хотим использовать
функцию `f` в файле `b.asm` а функцию `g` в файле `c.asm`.
То, что файл выставляет наружу другим модулям, мы отдельно декларируем как `global`.
То, что мы хотим импортировать из других файлов, в месте импорта декларируется как `extern`.

```nasm
; a.asm ; 
global f
global g

; h остаётся недоступной

f: 
   код 
g: 
   код 
h: 
   код 
```

```nasm
; b.asm ;
extern f

...
call f ; работает
```

```nasm
; c.asm ;
extern g

...
call g ; работает
```

На что это влияет: все метки на уровне ассемблера становятся частью таблицы
символов объектного файла.
Компоновщик называет метки **символами** (бывают и символы которые не
сопоставлены меткам).
Компоновщик использует таблицы символов чтобы связывать обращения по адресам и определения меток.


> **Вопрос** Мы будем изучать исполняемые и объектные файлы с помощью утилит
> `readelf`, `nm` и `objdump`. Разберитесь, как посмотреть таблицу символов в .o
> файле с помощью всех трёх. Каждый раз отметьте разницу между локальными и
> глобальными символами.


> **Вопрос** Что такое секции `.rodata` и `.bss`? Создайте ассемблерный файл с такими секциями и наполните их произвольными данными. Посмотрите таблицу секций с помощью `readelf --sections`. Какие флаги у этих секций и почему?

# Make-файлы

Объектные файлы, из которых собирается исполняемый файл, компилируются независимо. 
Значит в примере выше если мы изменили `a.asm` то не нужно перекомпилировать `b.asm` и `c.asm`. 
Достаточно шагов:

```sh
> nasm -f elf64 -o a.o a.asm
> ld -o program a.o b.o c.o
```

Чтобы автоматизировать процесс поиска изменений и не делать лишнюю работу
используют системы сборки, одна из наиболее распространённх это GNU Make.

Make имеет необычную  модель вычислений: мы описываем зависимости между файлами, а make просчитывает, в какой последовательности нужно выполнить задания по сборке файлов когда некоторые из них меняются.

Например, для тех же трёх файлов:

```make
# это комментарий

# формат каждого правила: 
# имя_файла_для_сборки: зависимость1 зависимость2 ...
# <обязательно символ tab> команда1
# <обязательно символ tab> команда2

a.o: a.asm
    nasm -f elf64 -o a.o a.asm

# b.o зависит от a.o но также и транзитивно от a.asm. 
# Не нужно указывать транзитивные зависимости, make их понимает

b.o: b.asm a.o
    nasm -f elf64 -o b.o b.asm

c.o: c.asm a.o
    nasm -f elf64 -o c.o c.asm
    
program: a.o b.o c.o
    ld -o program a.o b.o c.o
```

Сохраните это в `Makefile` и запустите `make program` чтобы выполнить действия по сборке цели `program`.

В таких файлах можно создавать переменные. Часто имена и флаги компиляторов, линкеров и т.д. указывают в переменных. Чтобы подставить значение переменной `var` пишут `$(var)`.

```make
# тот же файл с переменными

ASM=nasm
ASMFLAGS=-f elf64

a.o: a.asm
    $(ASM) $(ASMFLAGS) -o a.o a.asm

# b.o зависит от a.o но также и транзитивно от a.asm. 
# Не нужно указывать транзитивные зависимости, make их понимает

b.o: b.asm a.o
    $(ASM) $(ASMFLAGS) -o b.o b.asm

c.o: c.asm a.o
    $(ASM) $(ASMFLAGS) -o c.o c.asm
    
program: a.o b.o c.o
    ld -o program a.o b.o c.o
```

Советуем также прочитать про переменные `$@`, `$<`
> **Вопрос** Напишите простой makefile для вашей второй лабораторной работы.
> Используйте переменные чтобы задать имя ассемблера и флаги компиляции.
> [Книга](https://www.cl.cam.ac.uk/teaching/0910/UnixTools/make.pdf)


# Препроцессор

Чтобы понять, как препроцессор изменит вашу программу `m.asm`, запустите `nasm
-E m.asm`.

Для лабораторной работы вам потребуются следующие директивы:

- `%define`
- `%macro`
- `%+`
- `%%`

Прочитайте о них в документации `nasm` и в книге `Low-level programming` на стр. 64-73.

> **Вопрос** Чем отличаются директивы `define`, `xdefine`, `assign`? Придумайте пример, где они ведут себя по-разному и проиллюстрируйте с помощью `nasm -E`.

# Виртуальная память

- Возьмите программу на ассемблере с секциями `.rodata` и `.bss`, которую вы уже
  написали ранее. Модифицируйте её так, чтобы она при запуске входила в
  бесконечный цикл. Сделайте это за одну ассемблерную команду.

- Скомпилируйте и запустите эту программу в фоне (см. команды оболочки `fg`, `bg`, `jobs` и значение `Ctrl-Z`).
- Узнайте PID этой программы (например, с помощью `ps` или `top`)
- Зайдите в директорию `/proc/PID` где `PID` это идентификатор процесса.
- Изучите содержимое файла `maps`. Что это?


> **Вопрос** Прочитайте про виртуальную память стр 47-53. Затем обзорно про
> формат Page table entry. Какие биты из PTE вы наблюдаете в файле `maps`?

> **Вопрос** Что такое регион памяти?

> **Вопрос** Прочитайте о секциях и сегментах ELF-файлов на стр. 74-81. Как
> связаны сегменты и регионы памяти, которые вы увидели изучая `maps`?

> **Вопрос** Изучите секции 4.8 и 4.9. Модифицируйте пример так, чтобы сначала
> отобразить в память файл `filename.txt`, содержащий имя файла, а затем
> отобразить содержимое файла, чьё имя мы достали из `filename.txt`.

# Для выполнения лабораторной работы №2

* Прочитайте главы 4,5 из книги "Low-level programming". 
* Полное описание задания в секции 5.4
* От вас также требуется простейший `Makefile`
* Вопросы из книги из глав 1,2,4,5 могут быть спрошены на защите.
* Не забудьте просмотреть справочные страницы из мануала Intel ко всем инструкциям, которые вы использовали.


# Материалы

- [Intel® 64 and IA-32 Architectures Software Developer’s Manuals](https://software.intel.com/content/www/us/en/develop/download/intel-64-and-ia-32-architectures-sdm-combined-volumes-1-2a-2b-2c-2d-3a-3b-3c-3d-and-4.html)
- Appendix D из книги "Low-level programming"
- [Nasm manual](https://www.nasm.us/doc/)
- [Debugging with gdb](http://sourceware.org/gdb/current/onlinedocs/gdb/)
- [книга по make](https://www.cl.cam.ac.uk/teaching/0910/UnixTools/make.pdf)
- `man nasm`

