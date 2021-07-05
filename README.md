# Модель железной дороги

Модель железной дороги представляет ориентированный взвешенный граф, вершинами которого являются станции, а дугами - железные дороги, которые имеют строгое направление и протяженность (отражается весами). Так же вершинами могут быть железнодорожные переезды.

Моделирование происходит пошагово. Поезда ездят по специально заданному маршруту. 
Звенья цепи маршрута могут предусматривать следующие действия при прохождении через станцию:
- загрузка
- разгрузка
- транзит
- временная стоянка

Поезд состоит из локомотива и вагонов. Локомотивы могут отличаться друг от друга по тяговой силе или скорости и сроку службы. Чем больше к локомотиву прицеплено вагонов и чем больше они загружены, тем меньше скорость локомотива. Чем больше скорость локомотива и чем меньше вес дуги, тем быстрее поезд проходит от начала дуги до ее конца. 
Вагоны поезда бывают товарные и пассажирские, в то время, как станции бывают трех видов:
- пассажирские (пассажирские выгоны загружаются быстро за фиксированный срок)
- товарные (загружаются товарные вагоны)
- пассажирско-товарные (загружаются и товарные, и пассажирские вагоны)

Изначально на станции имеется какое-то начальное количество товарного или пассажирского ресурса и со временем оно обновляется. 
Объектная модель должна соответствовать следующим требованиям:
- включать классы видов поездов, вагонов и железнодорожных станций (количество на усмотрение разработчика)
- включать классы описания железной дороги (игрового поля)
- предусматривать механизм моделирования передвижения, загрузки и разгрузки поездов
- включать классы, которые обеспечивает разгрузку модели из текстового файла

Реализованная на основе разработанной модели программа должна:
- получать имя входного файла с описанием мира и описание режима работы (автоматическое моделирование или пошаговое моделирование с ожиданием подтверждения от пользователя) через аргументы командной строки
- выводить в консоль описания произошедших событий

Файл входных данных должен содержать:
- перечисление вершин графа железной дороги с указанием типа этого узла
- перечисление связей между вершинами графа железной дороги
- начальное положение, тип и перечисление вагонов поездов
- задание маршрутов движения для поездов
