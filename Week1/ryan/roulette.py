import random

SLOTS = 38


class Roulette(object):
    def __init__(self):
        self.weights = [SLOTS-1]*SLOTS

    def random(self):
        r = random.randrange(0, sum(self.weights))
        for i, x in enumerate(self.weights):
            r -= x
            if r < 0:
                result = i
                break
        # adjust weights
        for i, x in enumerate(self.weights):
            if x < SLOTS - 1:
                self.weights[i] += 1
        self.weights[result] -= SLOTS/2
        if result == SLOTS - 1:
            return '00'
        return str(result)
