from kivy.app import App
from kivy.uix.gridlayout import GridLayout
from kivy.uix.widget import Widget
from kivy.lang import Builder

from kivy.uix.button import Button

from kivy.uix.label import Label

from ai.ttt import State, ab

from kivy.clock import Clock

class TicTacToeGame():
    def __init__(self):
        self.state = State()
        self.state.player = 2 # ai is always player 2 so this is always the same
        self.state.min_v = 1
        self.state.max_v = 2
        self.grid = GridLayout(cols=3,
                               padding=20,
                               spacing=20)
        self.new_state_to_grid()

    def grid_to_state(self):
        pieces = map(lambda button: int(button.text), self.grid.children)
        self.state = []
        self.state += pieces[0:3]
        self.state += pieces[3:6]
        self.state += pieces[6:9]

    def new_state_to_grid(self):
        board = self.state.board
        for y, row in enumerate(board):
            for x, piece in enumerate(row):
                if piece == 0:
                    widget = Button(text=str(piece))
                    widget.self = self
                    widget.x_grid = x
                    widget.y_grid = y
                    widget.bind(on_press = callback)
                    self.grid.add_widget(widget)
                else:
                    widget = Label(text=str(piece))
                    self.grid.add_widget(widget)

    def state_to_grid(self, _):
        self.grid.clear_widgets()
        board = self.state.board
        for y, row in enumerate(board):
            for x, piece in enumerate(row):
                if piece == 0:
                    widget = Button(text=str(piece))
                    widget.self = self
                    widget.x_grid = x
                    widget.y_grid = y
                    widget.bind(on_press = callback)
                    self.grid.add_widget(widget)
                else:
                    widget = Label(text=str(piece))
                    self.grid.add_widget(widget)

def callback (button):
    x = button.x_grid
    y = button.y_grid
    self = button.self
    self.state.board[y][x] = 1
    self.state = ab(self.state)
    self.state.player = 2
    self.state.min_v = 1
    self.state.max_v = 2

class TTTApp(App):
    def build(self):
        tic_tac_toe_game = TicTacToeGame()
        grid = tic_tac_toe_game.grid
        Clock.schedule_interval(tic_tac_toe_game.state_to_grid, 1.0 / 60.0)
        return grid


if __name__ == '__main__':
    TTTApp().run()
