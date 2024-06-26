## Visitor & State
### Данный код представляет собой калькулятор, который использует паттерны Visitor и State для обработки введенного математического выражения и выполнения вычислений.

#### Visitor:
- В коде паттерн Visitor используется для обхода и обработки различных типов токенов.
- Есть класс TokenVisitor, который определяет методы для посещения каждого типа токена. Классы NumberToken, BraceToken, OperationToken имеют метод для принятия посетителя.
- Есть еще классы ParserVisitor, PrintVisitor, CalcVisitor, которые представляют конкретные действия: парсинг выражения, печать токенов и выполнение вычислений.

#### State:
- Паттерн State обеспечивается использованием паттерна Visitor, который позволяет добавлять новые операции над токенами без изменения самих токенов.

#### Сама программа:
- При запуске пользователь вводит математическое выражение, которое затем разбивается на токены с помощью Tokenizer.
- После происходит обработка токенов с помощью ParserVisitor, который строит обратную польскую нотацию для дальнейщих вычислений.
- С помощью PrintVisitor выводятся токены в обратной польской нотации в консоли.
- Вычисления выполняются с помощью CalcVisitor, который использует стек для хранения операндов и выполняет операции над ними.
- Результат после вычислений выводится в консоли.

Для измениеия цветов использовал библиотеку termcolor(https://github.com/ikalnytskyi/termcolor.git).