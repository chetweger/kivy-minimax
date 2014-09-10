from kivy.app import App
from kivy.uix.gridlayout import GridLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.widget import Widget
from kivy.lang import Builder

from kivy.uix.button import Button

from kivy.uix.label import Label

from ai.ttt import State, ab

from kivy.properties import ObjectProperty

class TTTButton(Button):
    def __init__(self, x, y, *args, **kwargs):
        super(TTTButton, self).__init__(*args, **kwargs)
        self.x_grid = x
        self.y_grid = y

class TicTacToeGame(Widget):
    frame = ObjectProperty(None)
    ai_first_button = ObjectProperty(None)
    def init(self):
        self.state = State()
        self.state.player = 2 # ai is always player 2 so this is always the same

        self.ai_first_button.bind(on_press = self.ai_plays_first)

        self.grid = GridLayout(cols=3,
                               padding=20,
                               spacing=20)

        self.frame.add_widget(self.grid)
        self.state_to_grid()

    def state_to_grid(self):
        self.grid.clear_widgets()
        board = self.state.board
        for y, row in enumerate(board):
            for x, piece in enumerate(row):
                if piece == 0:
                    button = TTTButton(x, y, text=str(piece))
                    button.bind(on_press = self.human_plays)
                    self.grid.add_widget(button)
                else:
                    widget = Label(text=str(piece))
                    self.grid.add_widget(widget)

    def human_plays(self, button):
        if self.state.max_piece == -1:
            print '\n\nTrue\n\n'
            # we know that the human is playing first!
            # therefore the AI will be player 2 (try to maximize utility of 2)
            self.frame.remove_widget(self.ai_first_button)
            self.state.max_piece = 2
            self.state.min_piece = 1
            self.state.next_piece = 2

        x = button.x_grid
        y = button.y_grid
        self.state.board[y][x] = self.state.min_piece
        self.state = ab(self.state)
        self.state.next_piece = self.state.max_piece
        self.state_to_grid()

    def ai_plays_first(self, button):
        button.parent.remove_widget(button) # once ai plays first, AI can't play first again.
        self.state.max_piece = 1
        self.state.min_piece = 2
        self.state.next_piece = 1
        self.state = ab(self.state)
        self.state.next_piece = self.state.max_piece
        self.state_to_grid()

class TTTApp(App):
    def build(self):
        tic_tac_toe_game = TicTacToeGame()
        tic_tac_toe_game.init()
        frame = tic_tac_toe_game.frame
        return frame

if __name__ == '__main__':
    TTTApp().run()

