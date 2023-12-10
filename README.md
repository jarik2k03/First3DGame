## Всем привет! 
Я решил в кои-то веки заняться чем-то полезным не вставая с дивана - создать небольшой игровой движок.
Так как это мой первый опыт в осваивании 3д графики, может получиться достаточно корявая сборка и низкопроизводительное фуфло, которое еще и не приучено *подтирать за собой*.
Впрочем, а у кого с первого раза получалось? Проект, само собой, пишется на языке ~~высокого уровня~~ C++ (a.k.a В&#9766;&#9766;).
Этот движок не обошёлся без готовых решений, которые, к слову, прямо из-под коробки Windows.

**Библиотеки, лежащие в основу ядра движка:**
### Microsoft DirectX SDK (June 2010):: Direct3D11
- Является основой 3D рендеринга с возможностью управлять буферами, настраивать интерфейсы для многообразного взаимодействия с шейдерами, написанными на языке HLSL.

### WinAPI 
- Понадобится для MessageBox-ов, отладки через консоль Visual Studio и обработки данных с клавиатуры и мыши.

### XNAMath
- DirectX библиотека, позволяющая взаимодействовать с матрицами и векторами, которые задают порядок перемещения и наблюдения в виртутальном мире.

Проект создан и адаптирован для работы в Visual Studio.
- [x] https://github.com/jarik2k03/First3DGame/issues/1
- [x] https://github.com/jarik2k03/First3DGame/issues/2
- [x] https://github.com/jarik2k03/First3DGame/issues/3
- [x] https://github.com/jarik2k03/First3DGame/issues/4
- [x] https://github.com/jarik2k03/First3DGame/issues/5
- [x] https://github.com/jarik2k03/First3DGame/issues/6
- [x] https://github.com/jarik2k03/First3DGame/issues/7
- [ ] https://github.com/jarik2k03/First3DGame/issues/8
