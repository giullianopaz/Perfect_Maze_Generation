'''
    Autor: Giulliano Lyra Paz
    E-mail: giulliano94@gmail.com

'''
from pygame.locals import *
import pygame
import sys

if len(sys.argv) < 3:
    print("""
    Você deve informar as dimensões do tabuleiro.

    Exemplo: $ python3 draw_maze.py 35 53

    OBS: As dimensões devem ser as mesmas da geração do labirinto no código em C++

    Considere ler o 'README.md' ou dar uma olhada no 'script.sh'\n""")
    exit()
 

# Classe para realizar jogadas do jogador
class Player:
    def __init__(self):
        self.x = 16
        self.y = 16
        self.speed = 1
 
    def move_right(self):
        self.x = self.x + self.speed
 
    def move_left(self):
        self.x = self.x - self.speed
 
    def move_up(self):
        self.y = self.y - self.speed
 
    def move_down(self):
        self.y = self.y + self.speed


# Classe para operações com o labirinto
class Maze:
    def __init__(self):
      # Pega dimensões do labirinto por argumento
       self.n_rows = int(sys.argv[1])
       self.n_colunms = int(sys.argv[2])+1

       # Lê arquivo onde encontra-se o labirinto gerado pelo código C++
       arq = open("perfect_maze.txt")
       # Transformar em uma lista
       self.maze = list(arq.read())

    # Método para desenhar Labirinto
    def draw_maze(self,display_surf,image_surf):
       bx = 0
       by = 0

       for _ in range(self.n_colunms*self.n_rows):
           # Se achar # na lista de posições do labirinto, desenha bloco
           if self.maze[ bx + (by*self.n_colunms) ] == '#':
               display_surf.blit(image_surf,( bx * 16 , by * 16))
          
           # Incrementa posição de x
           bx = bx + 1
           # Se chegou no fim da linha, passa pra próxima linha
           if bx > self.n_colunms-1:
               bx = 0 
               by = by + 1
 
 
class Game:
 
    window_width = 16*int(sys.argv[2])+2
    window_height = 16*int(sys.argv[1])+1
    player = 0
 
    def __init__(self):
        self._running = True
        self._display_surf = None
        self._image_surf = None
        self._block_surf = None
        self.player = Player()
        self.maze = Maze()
 
    def on_init(self):
        pygame.init()
        self._display_surf = pygame.display.set_mode((self.window_width,self.window_height), pygame.HWSURFACE)
 
        pygame.display.set_caption('Perfect Maze')
        self._running = True
        self._image_surf = pygame.image.load("caqui.png").convert()
        self._block_surf = pygame.image.load("block.png").convert()
 
    def on_event(self, event):
        if event.type == QUIT:
            self._running = False
 
    def on_loop(self):
        pass
 
    def on_render(self):
        self._display_surf.fill((0,0,0))
        self._display_surf.blit(self._image_surf,(self.player.x,self.player.y))
        self.maze.draw_maze(self._display_surf, self._block_surf)
        pygame.display.flip()
 
    def on_cleanup(self):
        pygame.quit()
 
    def on_execute(self):
        if self.on_init() == False:
            self._running = False
 
        while( self._running ):
            pygame.event.pump()
            keys = pygame.key.get_pressed()
 
            if (keys[K_RIGHT]):
                self.player.move_right()
 
            if (keys[K_LEFT]):
                self.player.move_left()
 
            if (keys[K_UP]):
                self.player.move_up()
 
            if (keys[K_DOWN]):
                self.player.move_down()
 
            if (keys[K_ESCAPE]):
                self._running = False
 
            self.on_loop()
            self.on_render()
        self.on_cleanup()
 
if __name__ == "__main__" :
    game = Game()
    game.on_execute()
