"""
RNG - Roulette Number Generator
Requirements:
1. Generate random numbers like a double-zero roulette wheel
2. Make something that "feels" random: numbers that have appeared recently are less like to appear
3. "Runs" should never happen.

@see http://thedailywtf.com/articles/introducing-the-lucky-deuce

Changelog:
----------------------------------------
2015-08-06 by @presidentsdaughter
Refactor to conform to coding guidelines.
Tests: PASS
----------------------------------------
2015-08-05 by @presidentsdaughter
Add doctests to ensure no runs. (Req #3)
Add Chi-Squared test for uniform distribution
----------------------------------------
2015-08-05 by @presidentsdaughter
Implemented requirements
----------------------------------------

(c)2015 The Lucky Deuce(tm) Casino. All rights reserved.

Transmittal, reproduction, dissemination and/or editing of this
source code as well as utilization of its contents and communication
thereof to others without express authorization are prohibited.
Offenders will be held liable for payment of damages.  All rights
created by patent grant or registration of a utility model or design
patent are reserved.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
"""
import random

__american_roulette_numbers_in_clockwise_order__list_str__ = \
    ['00', '27', '10', '25', '29', '12', '8', '19', '31', '18', '6', '21', '33', '16', '4', '23', '35', '14', '2',
     '0', '28', '9', '26', '30', '11', '7', '20', '32', '17', '5', '22', '34', '15', '3', '24', '36', '13', '1']

__size_of_american_roulette_int__ = len(__american_roulette_numbers_in_clockwise_order__list_str__)  # size = 38

__ZERO_CONST__INT__ = 0
__ONE_CONST__INT__ = 1
__TWO_CONST__INT__ = 2
__MEAN_CONST__FLOAT__ = 0.5
__STDDEV_CONST__FLOAT = 0.2


def spin():
    """
    Uses default Python Mersenne Twister RNG (53-bit precision floats, period 2**19937-1)
    See:
      M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-dimensionally equidistributed uniform
      pseudorandom number generator",
       ACM Transactions on Modeling and Computer Simulation Vol. 8, No. 1, January pp.3-30 1998.

    :return: Generator. Each call to next() will return next roulette value, as string

    Tests the generator for:
    Req #3: No runs. A run is defined if three (3) consecutive numbers are equal. There shouldn't be any
    >>> r = spin()
    >>> testN = 10000000
    >>> nums = [r.next() for n in xrange(testN)]
    >>> [(pos, val) for (pos, val) in enumerate(nums) if pos<(testN-3) and val==nums[pos+1]==nums[pos+2]]
    []

    # Test that implementation of Req #2 does not alter the true randomness of values.
    # CHI-Squared test
    # null hypothesis: uniform distribution. Goal: reject null hypothesis at 80% confidence
    # df = size - 1 = 37
    # Critical values for df = 37. @see http://www.stat.purdue.edu/~lfindsen/stat503/Chi-Square.pdf
    # p  0.80   0.90   0.95  0.96   0.97   0.975  0.98   0.99  0.995 0.9995
    # 43.978 48.363 52.192 53.344 54.781 55.668 56.730 59.893 62.883 71.97

    ## Can't even be at least 80% sure it is not uniform... :)
    >>> freqs=[nums.count(n) for n in __american_roulette_numbers_in_clockwise_order__list_str__]
    >>> Ei=testN*1.0/__size_of_american_roulette_int__
    >>> chi = sum([(Oi-Ei)**2/Ei for Oi in freqs])
    >>> chi < 43.978
    True
    """

    ##############################################################################################
    frequency_of_each_roulette_element__list_int = [__ZERO_CONST__INT__ for _ in
                                                    __american_roulette_numbers_in_clockwise_order__list_str__]
    random_position_in_array__int = random.randint(__ZERO_CONST__INT__,
                                                   __size_of_american_roulette_int__-__ONE_CONST__INT__)
    # initial value
    selected_value_from_roulette__str =\
        __american_roulette_numbers_in_clockwise_order__list_str__[random_position_in_array__int]
    yield selected_value_from_roulette__str
    while True:
        # store occurrence of value
        frequency_of_each_roulette_element__list_int[random_position_in_array__int] += __ONE_CONST__INT__
        if frequency_of_each_roulette_element__list_int[random_position_in_array__int] >= __TWO_CONST__INT__:
            # AVOID RUNS, acc. Req #3
            frequency_of_each_roulette_element__list_int[random_position_in_array__int] = __ZERO_CONST__INT__
            random_position_in_array__int =\
                (random_position_in_array__int + __ONE_CONST__INT__) % __size_of_american_roulette_int__
        else:
            # else proceed as usual
            # "walking on the closed wheel", i.e. Req #2
            # to avoid recent numbers, next number won't be from a uniform distribution,
            # but most likely (P>66%) 3..8 positions away - clockwise
            random_position_in_array__int = \
                (random_position_in_array__int +
                 int(random.gauss(__MEAN_CONST__FLOAT__, __STDDEV_CONST__FLOAT) *
                     (__size_of_american_roulette_int__/3))) % __size_of_american_roulette_int__
        # next value
        selected_value_from_roulette__str =\
            __american_roulette_numbers_in_clockwise_order__list_str__[random_position_in_array__int]
        yield selected_value_from_roulette__str

    #############################################################################################

if __name__ == "__main__":
    spinner = spin()
    N = int(raw_input("Test mode. Sequence length: "))
    print [spinner.next() for n in range(N)]
