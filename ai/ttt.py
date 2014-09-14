if __name__ == '__main__':
  print __name__
  import copy
  import sys
  import random
elif __name__ == 'ttt':
  pass# then we do not need to import these

EXACT = 'exact'
UPPER_BOUND = 'upper_bound'
LOWER_BOUND = 'lower_bound'
DIMENSION = 3

class Util:
  def __init__(self, value):
    self.value = value
  def __le__(self, other):
    return self.value >= other.value
  def __ge__(self, other):
    return self.value <= other.value
  def __gt__(self, other):
    return self.value > other.value
  def __lt__(self, other):
    return self.value < other.value

def utility(state):
  someone_won = state.is_win()
  if someone_won and state.next_piece == state.min_piece:
    value = 1 # utility is called before state.next_piece has moved, so really we are looking at the previous player
  elif someone_won and state.next_piece == state.max_piece:
    value = -1
  else:
    value = 0
  return value

class TTEntry:
  '''A TranspositionTable entry.
  definen methods to initialize and determine type
  so that entries from the TT table do not conflict with ab
  search.
  '''
  def __init__(self, value, a, b):
    if value > a and value < b:
      self. entry_type = EXACT
    elif value <= a:
      self.entry_type = UPPER_BOUND
    else:
      self.entry_type = LOWER_BOUND
    self.value = value

class TranspositionTable:
  def __init__(self):
    self.moves_table = {}
    self.entry_type = '' # we can have 'exact', 'upper_bound', or 'lower_bound'
  def call_min(self, state, a, b):
    potential_hit = state.serialize_me()
    if potential_hit in self.moves_table:
      entry = self.moves_table[potential_hit]
      if entry.entry_type == EXACT:
        return entry.value
      elif entry.entry_type == UPPER_BOUND:
        if b > entry.entry_type:
          b = entry.value
      else:
        if a > entry.entry_type:
          a = entry.value
      #assert entry.entry_type != ''
    miss = min_search(state, a, b, self)
    all_rotation_keys = state._serialize_all_rotations()
    for rotation in all_rotation_keys:
      self.moves_table[rotation] = TTEntry(miss, a, b)
    return miss
  def call_max(self, state, a, b):
    potential_hit = state.serialize_me()
    if potential_hit in self.moves_table:
      entry = self.moves_table[potential_hit]
      if entry.entry_type == EXACT:
        return entry.value
      elif entry.entry_type == UPPER_BOUND:
        if b > entry.entry_type:
          b = entry.value
      else:
        if a > entry.entry_type:
          a = entry.value
      #assert entry.entry_type != ''
    miss = max_search(state, a, b, False, self)
    all_rotation_keys = state._serialize_all_rotations()
    for rotation in all_rotation_keys:
      self.moves_table[rotation] = TTEntry(miss, a, b)
    return miss

def min_search(state, a, b, t_table):
  #print state.print_me()
  if state.is_over() or state.is_win():
    #print 'a', state.print_me()
    return utility(state)

  children = state.get_children()
  lowest = 101
  for child in children:
    #child.print_me()
    next_value = t_table.call_max(child, a, b)
    lowest = min(next_value, lowest)
    if next_value < b:
      b = next_value
    if next_value <= a:
      return -101
  return lowest

def max_search(state, a, b, is_root, t_table):
  #print state.print_me()
  if state.is_over() or state.is_win():
    #print 'b', state.print_me()
    return utility(state)

  children = state.get_children()
  child_util_pairs = zip(children, [Util(-100) for i in range(len(children))])
  child_util_pairs = map( list, child_util_pairs )
  highest = -101
  for i in range(len(child_util_pairs)):
    next_value = t_table.call_min(child_util_pairs[i][0], a, b)
    child_util_pairs[i][1].value = next_value
    highest = max(next_value, highest)
    if next_value > a:
      a = next_value
    if next_value >= b:
      return 101
  if is_root:
    print 'length of child_util_pairs is', len(child_util_pairs)
    max_states = [s for s in child_util_pairs if s[1].value == highest]
    #assert max_states # we don't want it to be empty!
    chosen_child = max_states[0]
    print 'length of max_s is ', len(max_states), map( lambda x: x[1].value, max_states )
    return chosen_child
  else:
    return highest

def minimax_search(state):
  '''Find next best move for the current next_piece.
  '''
  print 'minimax_search(state)', state.min_piece
  transposition_table = TranspositionTable()
  next_move = max_search(state, -9000, 9000, True, transposition_table)
  return next_move[0]

def generate_next_piece(next_piece):
  if next_piece == 1:
    return 2
  elif next_piece == 2:
    return 1
  assert False

class State:
  def __init__(self):
    self.board = [[0 for i in range(3)] for i in range(3)]
    self.next_piece = -1 # the next piece to be played (played by the AI)
    self.max_piece = -1  # the piece of the player the AI is trying to make win (maximize utility)
                         # E.G.: if AI is player 2 then max_piece = 2
    self.min_piece = -1  # the AI's opponent (AI tries to minimize this player's utility)

  def get_children(self):
    children = []
    for row_index in range(3):
      for column_index in range(3):
        if self.board[row_index][column_index] == 0:
          child_state = self.copy_me()
          child_state.board[row_index][column_index] = self.next_piece
          child_state.next_piece = generate_next_piece(self.next_piece)
          children += [child_state]
    return children

  def is_over(self):
    for row in self.board:
      for piece in row:
        if piece == 0:
          return False
    return True

  def is_win(self):
    board = self.board
    diagonal_1 = [board[i][i] for i in range(3)]
    diagonal_2 = [board[i][2-i] for i in range(3)]
    copy_board = self.copy_me().board
    for row in copy_board:
      if bool(reduce( (lambda cell_x, cell_y: cell_x & cell_y), row)):
        return row[0]
    transposed_board = zip(*copy_board)
    for column in transposed_board:
      if bool(reduce( (lambda cell_x, cell_y: cell_x & cell_y), column)):
        return column[0]
    if bool(reduce( (lambda cell_x, cell_y: cell_x & cell_y), diagonal_1)):
      return diagonal_1[0]
    if bool(reduce( (lambda cell_x, cell_y: cell_x & cell_y), diagonal_2)):
       return diagonal_2[0]
    return 0

  def check_win(self):
    '''Returns 1 or 2 if either of these players won or returns 0 if
    neither player won in the case of a tie
    '''
    board_full = self.is_over()
    winning_player = self.is_win()
    if winning_player:
      return winning_player
    elif board_full:
      return -1 # nobody won
    else:
      return 0

  def serialize_me(self):
    return _serialize_board(self.board)

  def print_me(self):
    print 'current next_piece to play: ', self.next_piece, ' is win ', str(self.is_win()), '\nboard:'
    for row in self.board:
      print row
    print

  def _serialize_all_rotations(self):
    '''All the rotations:

       111     111     000     000
       001     100     001     100
       000(r0) 000(r1) 111(r2) 111(r3)

       100     001     110     011
       100     001     100     001
       110(r4) 011(r5) 100(r6) 001(r7)
    '''
    r0 = self.board[:3] # make copy
    r1 = map( lambda row: reverse(row), r0 )
    r1 = map( list, r1 )
    r2 = reverse(r0)
    r3 = map( lambda row: reverse(row), r2 )

    r4 = zip(*r0)
    r4 = map( list, r4 )
    r5 = map( lambda row: reverse(row), r4 )
    r5 = map( list, r5 )
    r6 = reverse(r4)
    r7 = map( lambda row: reverse(row), r6 )

    #return r0, r1, r2, r3, r4, r5, r6, r7

    all_boards_serialized = map( _serialize_board, (r0, r1, r2, r3, r4, r5, r6, r7) )
    all_boards_serialized = list(set(all_boards_serialized)) # make unique
    return all_boards_serialized

  def copy_me(self):
    copy = State()
    for r in range(DIMENSION):
      for c in range(DIMENSION):
        copy.board[r][c] = self.board[r][c]
    copy.next_piece = self.next_piece
    copy.max_piece = self.max_piece
    copy.min_piece = self.min_piece
    return copy


def _unserialize_board(string_board):
  list_board = []
  row_0 = map( int, list(string_board[:3]) )
  row_1 = map( int, list(string_board[3:6]) )
  row_2 = map( int, list(string_board[6:]) )
  list_board = [row_0, row_1, row_2]
  return list_board

def unhash(string_board):
  a = State()
  a.board = _unserialize_board(string_board)
  return a

def reverse(board):
  board = board[:len(board)] # make copy
  board.reverse()
  return board

def _serialize_board(board):
  str_board = ''
  for row in board:
    #str_board += '|'
    for piece in row:
      str_board += (str(piece))
  return str_board # * 2
