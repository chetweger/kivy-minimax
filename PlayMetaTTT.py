from kivy.app import App
from kivy.uix.gridlayout import GridLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.widget import Widget
from kivy.lang import Builder

from kivy.uix.button import Button

from kivy.uix.label import Label

from ai.learning import State, minimax_search, is_win, is_full

from kivy.properties import ObjectProperty

from kivy.uix.scrollview import ScrollView

class MetaTTTButton(Button):
    def __init__(self, *args, **kwargs):
        super(MetaTTTButton, self).__init__(*args, **kwargs)
        self.meta_x = kwargs['meta_x'] # x location on meta board
        self.meta_y = kwargs['meta_y'] # y location on meta board
        self.mini_x = kwargs['mini_x'] # x location on mini board
        self.mini_y = kwargs['mini_y'] # y location on mini board

class TicTacToeGame(Widget):
    window_frame = ObjectProperty(None) # root object that hold everything
    game_space = ObjectProperty(None)
    toolbar = ObjectProperty(None)
    ai_first_button = ObjectProperty(None)
    new_game = ObjectProperty(None)
    meta_board = ObjectProperty(None)
    game_info = ObjectProperty(None)


    td_consts = {'c3': 0.767944, 'c2': 1.049451, 'c1': 3.074038, 'c6': 0.220823, 'c5': 0.181883, 'c4': 0.605861}

    def initialize(self, app):
        self.state = State()
        self.ai_first_button.bind(on_press = self.ai_plays_first)
        self.new_game.bind(on_press = self.start_new_game)
        self.state_to_grid()
        self.app = app
        self.game_info.text = 'Scroll: AI: %d | Human %d' % (0, 0)

    def start_new_game(self, button):
        # TODO: avoid this method by moving TicTacToeGame beneath a mother game method.
        self.app.stop()
        PlayMetaTTTApp().run() # start a new game!

    def will_buttons(self, meta_y, meta_x):
        # first we determine if the next_piece points to a playable board.
        board = self.state.boards
        piece = list(self.state.next_piece)
        playable = True
        if is_win(board[piece[0]][piece[1]]) or is_full(board[piece[0]][piece[1]]):
            playable = False
        if (not is_win(board[meta_y][meta_x])) and (not is_full(board[meta_y][meta_x])):
            if not playable:
                return True
            if playable:
                return (meta_y == piece[0]) and (meta_x == piece[1])
        return False

    def update_score(self, button):
        if is_win(self.state.boards[button.meta_y][button.meta_x]):
            # update human score
            self.state.score[str(self.state.min_piece)] += 1
        self.game_info.text = ('Scroll: AI: %d | Human %d' %
                               (self.state.score[str(self.state.max_piece)],
                                self.state.score[str(self.state.min_piece)]))

    def state_to_grid(self):
        self.meta_board.clear_widgets()
        boards = self.state.boards
        for meta_y, row_meta_boards in enumerate(boards):
            for meta_x, meta_board in enumerate(row_meta_boards):

                # can the human play on this mini board?
                # if yes, make buttons on free spaces (0's)
                # if no, make dashes on free spaces (0's)
                will_make_buttons = self.will_buttons(meta_y, meta_x)

                mini_board = GridLayout(cols = 3)

                for mini_y, row in enumerate(meta_board):
                    for mini_x, piece in enumerate(row):
                        if piece['cell'] == 0:
                            if will_make_buttons:
                                button = MetaTTTButton(text = str(piece['cell']),
                                                       meta_y = meta_y,
                                                       meta_x = meta_x,
                                                       mini_y = mini_y,
                                                       mini_x = mini_x)
                                button.bind(on_press = self.human_plays)
                                mini_board.add_widget(button)
                            else:
                                widget = Label(text=str('-'))
                                mini_board.add_widget(widget)
                        else:
                            mini_board.add_widget(Label(text=str(piece['cell'])))

                self.meta_board.add_widget(mini_board)


    def do_nothing(self, button):
        '''A disabled button callback used for when game is over.
        '''
        pass

    def check_win(self):
        winning_player = self.state.check_winner() # returns 1 or 2 if either of these players won or returns 0 if
                                                # neither player won in the case of a tie

        if winning_player:
            self.human_plays = self.do_nothing # deactivate button callback.

            if winning_player == int(self.state.min_piece):
                # if this case is true, the human won!??!
                # Wow, this should never happen! let the player know they discovered a bug in my code.
                winning_text = '\n\nCongradualations, you won!'
            elif winning_player == int(self.state.max_piece):
                # ai won
                winning_text = '\n\nYou lost.  Better luck next time.'
            else:
                winning_text = '\n\nThe game ended in a tie.'
            self.game_info.text += winning_text
        return winning_player

    def human_first(self):
        self.state.max_piece = '2'
        self.state.min_piece = '1'
        self.state.next_piece[2] = 2

    def ai_first(self):
        self.state.max_piece = '1'
        self.state.min_piece = '2'
        self.state.next_piece[2] = 1

    def set_next_state(self, button):
        self.state.next_piece[0] = button.mini_y
        self.state.next_piece[1] = button.mini_x
        self.state.next_piece[2] = int(self.state.max_piece)

    def human_plays(self, button):
        if self.state.max_piece == -1:
            # we know that the human is playing first!
            # therefore the AI will be player 2 (try to maximize utility of 2)
            self.toolbar.remove_widget(self.ai_first_button) # once ai plays first, AI can't play first again.
            self.human_first()

        self.state.boards[button.meta_y][button.meta_x][button.mini_y][button.mini_x]['cell'] = int(self.state.min_piece)
        self.set_next_state(button)

        self.update_score(button)
        if self.check_win(): # check if the human just won
            self.state_to_grid()
            return

        expected_utility, self.state = minimax_search(self.state, self.td_consts)
        self.check_win() # check if the AI just won
        self.state_to_grid()


    def ai_plays_first(self, button):
        self.toolbar.remove_widget(button) # once ai plays first, AI can't play first again.
        self.ai_first()
        expected_utility, self.state = minimax_search(self.state, self.td_consts)
        self.state_to_grid()

class PlayMetaTTTApp(App):
    def build(self):
        tic_tac_toe_game = TicTacToeGame()
        tic_tac_toe_game.initialize(self)
        game_space = tic_tac_toe_game.window_frame
        return game_space

if __name__ == '__main__':
    PlayMetaTTTApp().run()
