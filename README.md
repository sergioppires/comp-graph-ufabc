# Computação Gráfica - UFABC
## Nome: Sérgio Abilio | RA: 11009513

----

## Atividade 2 - Aplicação gráfica: Mushroom Dreams

[Jogo em WebAssembly](https://sergioppires.github.io/comp-graph-ufabc/apps/mushroom_dream/index.html)

### Resumo
É uma aplicação demo 3d que possui um cogumelo central e diversos outros cogumelos de fundo findo em direção à camera. Você pode interagir com o objeto central usando seu mouse, assim você pode rotaciona-lo do jeito que quiser. As cores mudam automaticamente, mas você pode desabilitar essa função no widget menu. <b> Recomendamos que a visualização seja feita em fullscreen.</b>

### Como interagir?
Use o mouse para interagir com o cogumelo central.

### Implementações

<b>Cogumelo Central</b>: É o personagem principal da aplicação.

<b>Efeito starfield</b>: O efeito starfield foi feito com base no [efeito dado em aula](https://hbatagelo.github.io/cg/starfield.html). Esse efeito cria diversos objetos com uma escala menor que vem de encontro ao observador/camera. Quando os objetos passam um pouco a posição da câmera, eles são transladados até o começo do campo, gerando uma impressão de infinito.

<b>Efeito de cores</b>: As cores do background e dos cogumelos são opostas no circulo cromático, ou seja, são complementares. Esse efeito de cor gera um efeito psicodélico na aplicação.

<b>Cores aleatórias</b>: As cores são aleatoriezadas com o tempo para deixar a aplicação mais interessante. Também é possível desabilitar essa feature afim de fazer o manejo das cores manualmente.

<b>Face Culling</b>: Seguindo os exemplos dado em aula, por padrão, não renderizamos a parte interna dos cogumelos. Mas você pode alterar essa opção no widget menu.

<b>Créditos:<p> 
Modelo do cogumelo:</b> https://free3d.com/3d-model/short-strange-mushroom-v1--889995.html




----

## Atividade 1 - Jogo: Dogdge Ship

[Jogo em WebAssembly](https://sergioppires.github.io/comp-graph-ufabc/apps/ship_run/index.html)

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
