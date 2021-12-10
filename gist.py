from collections import defaultdict
def calculate_fish_population(fish, days):
    """
    Calculate the fish population after a given number of days.
    Each day, the fish born are parented by some old fish that also gave birth 7 days ago, or some new fish that were born 9 days ago.
    We can use dynamic programming to build a table - so the new fish a given day are born[day - 7] + born[day - 9].
    Since we initiate the fish population (based on the input) we also have to add any initial fish (born[day]).
    This is way way faster than simulating, simulating takes forever for 256 days.
    The total fish population is the sum of all the fish born + all the initial fish
    """
    born_at = defaultdict(lambda: 0)

    for fish_individual in fish:
        born_at[fish_individual] += 1
    for day in range(days):
        born_at[day] = born_at[day] + born_at[day - 7] + born_at[day - 9]
    return sum(born_at.values()) + len(fish)

if __name__ == "__main__":
    with open("/home/jamez-amd-ubuntu/gists/input/day6_ex.in", "r", encoding="utf-8") as ifile:
        livestock = list(map(int, ifile.read().split(",")))
    print(calculate_fish_population(livestock, 256))