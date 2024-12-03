import numpy as np
from typing import List, Tuple
import random


"""
    @class: MusicDataGenerator
    
    @brief: Generates musical frequency sequences with preference scores based on consonance/dissonance.
"""
class MusicDataGenerator:
    def __init__(self):

        # Base frequencies for octave 4 (middle C to B)
        self.base_frequencies = {
            'C': 261.63, 'C#': 277.18, 'D': 293.66, 'D#': 311.13,
            'E': 329.63, 'F': 349.23, 'F#': 369.99, 'G': 392.00,
            'G#': 415.30, 'A': 440.00, 'A#': 466.16, 'B': 493.88
        }

        # Define consonant intervals (frequency ratios)
        self.consonant_ratios = {
            'unison': 1 / 1,  # Most consonant
            'octave': 2 / 1,  # Most consonant
            'perfect_fifth': 3 / 2,  # Very consonant
            'perfect_fourth': 4 / 3,  # Consonant
            'major_third': 5 / 4,  # Consonant
            'minor_third': 6 / 5,  # Consonant
            'major_sixth': 5 / 3,  # Consonant
        }

        # Define more dissonant intervals
        self.dissonant_ratios = {
            'minor_second': 16 / 15,  # Very dissonant
            'major_seventh': 15 / 8,  # Dissonant
            'tritone': 45 / 32,  # Historically considered very dissonant
        }

    """
        @brief: calculate_consonance_score
        
        @param: frequencies -> type: List[float], list of frequencies (in Hz).
        @return: score -> type: float, value between 0 (most dissonant) and 1 (most consonant).
    """
    def calculate_consonance_score(self, frequencies: List[float]) -> float:
        score = 0
        comparisons = 0

        # Compare each frequency pair
        for i in range(len(frequencies)):
            for j in range(i + 1, len(frequencies)):
                ratio = max(frequencies[i], frequencies[j]) / min(frequencies[i], frequencies[j])

                # Find the closest matching ratio from our defined intervals
                min_diff = float('inf')
                for consonant_ratio in self.consonant_ratios.values():
                    diff = abs(ratio - consonant_ratio)
                    min_diff = min(min_diff, diff)

                # Convert difference to a score (smaller difference = higher score)
                pair_score = 1 / (1 + 10 * min_diff)  # Sigmoid-like scaling
                score += pair_score
                comparisons += 1

        return score / comparisons if comparisons > 0 else 0

    """
        @brief: Generates a sequence with a bias towards consonant intervals (classical style).
        @return: tuple -> type: Tuple[List[float], float], (frequencies, preference_score).
    """
    def generate_classical_sequence(self) -> Tuple[List[float], float]:
        frequencies = []
        base_freq = random.choice(list(self.base_frequencies.values()))
        frequencies.append(base_freq)

        # Generate remaining frequencies with bias towards consonant intervals
        for _ in range(9):
            prev_freq = frequencies[-1]
            if random.random() < 0.7:  # 70% chance of consonant interval
                ratio = random.choice(list(self.consonant_ratios.values()))
            else:
                ratio = random.choice(list(self.dissonant_ratios.values()))

            new_freq = prev_freq * ratio
            # range check for frequency
            while new_freq > 1000:
                new_freq /= 2
            while new_freq < 200:
                new_freq *= 2

            frequencies.append(new_freq)

        consonance_score = self.calculate_consonance_score(frequencies)
        preference_score = 0.8 * consonance_score + 0.2  # Classical music bias

        return frequencies, preference_score
    """
        @brief: Generates a sequence with a greater tolerance for dissonance (jazz style).
        
        @return: tuple -> type: Tuple[List[float], float], (frequencies, preference_score).
    """
    def generate_jazz_sequence(self) -> Tuple[List[float], float]:
        frequencies = []
        base_freq = random.choice(list(self.base_frequencies.values()))
        frequencies.append(base_freq)

        # Generate remaining frequencies with more tolerance for dissonance
        for _ in range(9):
            prev_freq = frequencies[-1]
            if random.random() < 0.4:  # 40% chance of consonant interval
                ratio = random.choice(list(self.consonant_ratios.values()))
            else:
                ratio = random.choice(list(self.dissonant_ratios.values()))

            new_freq = prev_freq * ratio
            while new_freq > 1000:
                new_freq /= 2
            while new_freq < 200:
                new_freq *= 2

            frequencies.append(new_freq)

        consonance_score = self.calculate_consonance_score(frequencies)
        preference_score = 0.5 * consonance_score + 0.5  # More accepting of dissonance

        return frequencies, preference_score

    """
        @brief: Generates a dataset of frequency sequences and their preference scores.
        
        @param: size -> type: int, number of sequences to generate.
        @param: style -> type: str, can be 'classical', 'jazz', or 'mixed'.
        @return: dataset -> type: List[Tuple[List[float], float]], list of sequences and scores.
    """
    def generate_dataset(self, size: int, style: str = 'mixed') -> List[Tuple[List[float], float]]:
        dataset = []
        for _ in range(size):
            if style == 'classical':
                sequence = self.generate_classical_sequence()
            elif style == 'jazz':
                sequence = self.generate_jazz_sequence()
            else:  # mixed
                if random.random() < 0.5:
                    sequence = self.generate_classical_sequence()
                else:
                    sequence = self.generate_jazz_sequence()
            dataset.append(sequence)
        return dataset


#Example usage
# if __name__ == "__main__":
#     generator = MusicDataGenerator()
#
#     # Generate different datasets
#     classical_dataset = generator.generate_dataset(10, 'classical')
#     jazz_dataset = generator.generate_dataset(10, 'jazz')
#     mixed_dataset = generator.generate_dataset(10, 'mixed')
#
#     # Print example sequences
#     print("Classical sequence example:")
#     freq, score = classical_dataset[0]
#     print(f"Frequencies: {[round(f, 2) for f in freq]}")
#     print(f"Preference Score: {round(score, 3)}")
#
#     print("\nJazz sequence example:")
#     freq, score = jazz_dataset[0]
#     print(f"Frequencies: {[round(f, 2) for f in freq]}")
#     print(f"Preference Score: {round(score, 3)}")