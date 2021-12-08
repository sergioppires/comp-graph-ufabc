# Computação Gráfica - UFABC
## Nome: Sérgio Abilio | RA: 11009513

## Atividade 3 - Aplicação gráfica Interariva: Crab World
:crab:

//TODO colocar GIF

[<h2>Jogo em WebAssembly</h2>](https://sergioppires.github.io/comp-graph-ufabc/apps/crab_world)

### Resumo
É uma aplicação demo 3d interativa que possui um caranguejo com um óculos de sol central e diversos outros caranguejos de fundo. Todos os objetos da cena possuem o efeito "cromado", ou seja, refletem o ambiente. O ambiente é composto por um cubemaps que representa uma praia. Você pode interagir com o objeto central usando seu mouse, assim você pode rotaciona-lo do jeito que quiser.<b> Recomendamos que a visualização seja feita em fullscreen.</b>

### Como interagir?
Use o mouse para interagir com o caranguejo central.

### Implementações

<b>Caranguejos</b>: É o personagem principal da aplicação. Ele foi feito a partir de uma composição de dois objetos via blender. Para fazer a composição dos objetos, usamos um modelo .OBJ do caranguejo e do óculos. Dentro do software, atualizamos seu eixo de posição, rotações e escalas para que fosse transmitida uma sensação de que ele estava usando o óculos de sol.

//TODO gif do blender

Nele foi aplicado o efeito de [trackball](https://hbatagelo.github.io/cg/vtrackball1.html) dado em aula. Esse efeito consiste em alterar a rotação do objeto de acordo com os cliques do mouse. Também é possível utilizar o scroll para fazer com que a cãmera fique mais perto do objeto ou mais longe.

<b>Efeito starfield</b>: O efeito starfield foi feito com base no [efeito dado em aula](https://hbatagelo.github.io/cg/starfield.html). Esse efeito cria diversos objetos com uma escala menor que vem de encontro ao observador/camera. Quando os objetos passam um pouco a posição da câmera, eles são transladados até o começo do campo, gerando uma impressão de que estamos gerando infinitos caranguejos. Porém apenas alocamos 100 objetos na memória e atualizamos sua posição de acordo com a necessidade.

<b>Textura e Iluminação</b>: O efeito de textura e iluminação espelhados foram feitos com base no modelo dado em aula

<p>
Créditos: </br>
Modelo do caranguejo: <a>https://free3d.com/3d-model/crab-baby-happy-v1--224697.html</a> </br>
Modelo do óculos: <a>https://free3d.com/3d-model/sunglasses-v1--803862.html</a></br>
Modelo do cubemap: <a>http://www.humus.name/index.php?page=Cubemap&item=NissiBeach</a>
</br>

----

## Atividade 2 - Aplicação gráfica: Mushroom Dreams
:mushroom:

![](assets/atividade2.gif)

[<h2>Jogo em WebAssembly</h2>](https://sergioppires.github.io/comp-graph-ufabc/apps/mushroom_dream)

### Resumo
É uma aplicação demo 3d que possui um cogumelo central e diversos outros cogumelos de fundo findo em direção à camera. Você pode interagir com o objeto central usando seu mouse, assim você pode rotaciona-lo do jeito que quiser. As cores mudam automaticamente, mas você pode desabilitar essa função no widget menu. <b> Recomendamos que a visualização seja feita em fullscreen.</b>

### Como interagir?
Use o mouse para interagir com o cogumelo central.

### Implementações

><b>Cogumelo Central</b>: É o personagem principal da aplicação. Nele foi aplicado o efeito de [trackball](https://hbatagelo.github.io/cg/vtrackball1.html) dado em aula. Esse efeito consiste em alterar a rotação do objeto de acordo com os cliques do mouse. Também é possível utilizar o scroll para fazer com que a cãmera fique mais perto do objeto ou mais longe.

><b>Efeito starfield</b>: O efeito starfield foi feito com base no [efeito dado em aula](https://hbatagelo.github.io/cg/starfield.html). Esse efeito cria diversos objetos com uma escala menor que vem de encontro ao observador/camera. Quando os objetos passam um pouco a posição da câmera, eles são transladados até o começo do campo, gerando uma impressão de que estamos gerando infinitos cogumelos. Porém apenas alocamos 500 objetos na memória e atualizamos sua posição de acordo com a necessidade.

><b>Efeito de cores</b>: As cores do background e dos cogumelos são opostas no circulo cromático, ou seja, são complementares. Esse efeito de cor gera um efeito psicodélico na aplicação. As cores são definidas na estrutura do RGB. As variaveis red, green e blue aceitam valores em formato de float entre 0 e 1 e são definidos no openglwindow.hpp. De acordo com a variação dessas cores variamos a cor do material aplicado no cogumelo, passando uma impressão de colorização. Para as cores do background utilizamos a cor complementar que é dada pela fórmula (corBackground = 1- corCogumelo) para o vermelho, verde e azul.

><b>Cores aleatórias</b>: As cores são aleatoriezadas com o tempo para deixar a aplicação mais interessante. Também é possível desabilitar essa feature afim de fazer o manejo das cores manualmente. Usamos essa implementação a partir da variação dos valores das cores red green e blue no openglwindow.

><b>Face Culling</b>: Seguindo os exemplos dado em aula, por padrão, não renderizamos a parte interna dos cogumelos. Mas você pode alterar essa opção no widget menu.

><b>Perspectiva</b>: Por padrão, nossos efeitos necessitam de perspectiva para fazerem sentido, pois estamos trabalhando com um conceito de profudindidade de visão. Existe a aopção de visão ortogonal apenas para fins didáticos, uma vez que a ilusão que criamos não funciona da maneira que desejamos nesse tipo de perspectiva.

><b>Créditos:<p> 
Modelo do cogumelo:</b> https://free3d.com/3d-model/short-strange-mushroom-v1--889995.html

----

## Atividade 1 - Jogo: Dogdge Ship

[Jogo em WebAssembly](https://sergioppires.github.io/comp-graph-ufabc/apps/ship_run)

### Resumo
Você deve escapar as naves triangulares do mal pelo meior tempo possível. Quanto mais tempo você desviar, maior será sua pontuação.

### Como jogar?
Pressione o botão esquerdo do seu mouse ou a barra de espaço para subir. Desvie dos objetos e ganhe muitos pontos.

### Implementações

<b>Nave</b>: É o seu personagem principal. Ela está fixada no eixo x e apenas se movimenta no eixo y. Existe uma força gravitacional que a puxa para baixo, mas caso um clique seja efetuado, essa gravidade é alterada de direção e a nave vai para cima.

<b>Obstaculos</b>: Obstaculos são triangulos que vão em direção ao jogador. O objetivo do jogo é desviar deles

<b>Progressão</b>: Quanto mais o tempo passa, maior sua pontuação e mais dificil fica o desafio. Quanto maior o level do seu jogo, mais rápido os obstáculos vão aparecer.


# Sobre o ABCg
Esse repositório foi criado como intuito de ser uma referência futura de implementações usando a lib ABCg.

# Tecnologia Usada

ABCg
======

Development framework accompanying the course MCTA008-17 Computer Graphics at [UFABC](https://www.ufabc.edu.br/).

----

### Credits

Developed by Harlen Batagelo.

### License

ABCg is licensed under the MIT License. See [LICENSE](https://github.com/hbatagelo/abcg/blob/main/LICENSE) for more information.
