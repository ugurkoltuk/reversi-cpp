#!/usr/bin/python

from subprocess import call
import random
import sys
import os

def string_from_settings(settings_list):
    return ', '.join(str(_) for _ in settings_list)

#settings: p, m, c, s
FNULL = open(os.devnull, 'w')

white_settings = [5, 10, 56, 24]
black_settings = [5, 10, 56, 24]
winner=1
for _ in range(1, 1000):
    print("Round " + str(_) + " / 1000", file=sys.stderr)
    winner = call(["./reversi-cpp", "cc", "4", 
            str(white_settings[0]), str(white_settings[1]), str(white_settings[2]), str(white_settings[3]), 
            str(black_settings[0]), str(black_settings[1]), str(black_settings[2]), str(black_settings[3])], 
            stderr=FNULL) 
    print("white_settings: (" + string_from_settings(white_settings) + ") black_settings: (" + string_from_settings(black_settings) + "). Result is: ", file=sys.stderr)
    number_of_weights_to_change = random.randint(1,4)
    weight_indices_to_change=random.sample(range(0,4), number_of_weights_to_change)
    
    if winner == 1:
        print("White wins.", file=sys.stderr)
        victim_settings=black_settings    
    if winner == 2:
        print("Black wins.", file=sys.stderr)
        victim_settings=white_settings

    for weight_index_to_change in weight_indices_to_change:
        victim_settings[weight_index_to_change] = victim_settings[weight_index_to_change] + random.randint(-10,10)
        if victim_settings[weight_index_to_change] < 0:
            victim_settings[weight_index_to_change] = 0 
    sys.stderr.flush()     
            
                    
    
      
