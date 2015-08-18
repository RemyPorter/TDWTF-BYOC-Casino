//  Created By: Josip Matić

using System;
using System.Text;
using System.IO;

namespace LuckyDeuceCasino
{
    class Program
    {
        Random r;
        int[] numberArray = new int[numberArrayAmount]; // used for 2nd roll
        int number;
        string path;

        const int numberArrayAmount = 1000;
        const int numbersToModify = 100; // Determines probability of 2nd number being equal to first number.
                                         // The higher the number, lesser probability is.
        const int maxNumber = 1000; // Limits the range of the randomizer, though it works with Random.Next() without problems.
                                    // This is mostly for simplicity's sake.

        static void Main(string[] args)
        {
            // I have an unexplained aversion towards static
            Program p = new Program();
            p.Start();
        }

        private void Start()
        {
            r = new Random(); // Starts a new randomizer

            // Randomizer is too predictable, and we don't want anyone winning jackpot
            // It's more random if we write random numbers to file
            path = Directory.GetCurrentDirectory() + "\\temp.file";

            do
            {
                // Stop the first wheel
                number = r.Next(maxNumber);
                Console.Write(number);

                // To hide it from folk we're rippi- I mean, providing entertainment to,
                // create a file now
                WriteRandom();
                System.Threading.Thread.Sleep(3000);

                // Stop the second wheel
                Console.Write("   ");
                Console.Write(SecondRoll(number));
                System.Threading.Thread.Sleep(4000);

                // Stop the third wheel; number is determined by reading from file
                ReadRandom();
                Console.Write("   ");
                Console.WriteLine(number);

                // Hide randomization method.
                File.Delete(path);
            } while (Console.ReadKey(true).Key != ConsoleKey.Escape);
        }

        private int SecondRoll(int n)
        {
            // We're filling the probability array with first number...
            for (int i = 0; i < numberArray.Length; i++)
                numberArray[i] = n;

            // ...then choose the random cells in array to fill with other numbers...
            for (int i = 0; i < numbersToModify; i++)
                numberArray[r.Next(numberArrayAmount)] = r.Next(maxNumber);

            // ...and finally, pick one of numbers. Due to majority of numbers being the first number,
            // probability of second number being the same as first number is very high.
            // Just like those bastards wanted.
            return numberArray[r.Next(numberArrayAmount)];
        }

        private void WriteRandom()
        {
            using (TextWriter writer = new StreamWriter(path))
            {
                int count = r.Next(1000); // Choose random amount of numbers to write.

                // Then, write that amount of random numbers into the text file.
                for (int i = 0; i < count; i++)
                    writer.Write(Convert.ToString(r.Next(maxNumber)));
            }
        }

        private void ReadRandom()
        {
            try
            {
                using (TextReader reader = new StreamReader(path))
                {
                    // First, read all digits into one string
                    string s = reader.ReadToEnd();

                    // The, take out a number from that giant string
                    int numberOfDigits = (int)(Math.Log10(maxNumber));
                    int offset = r.Next(s.Length - numberOfDigits);
                    string num = s.Substring(offset, numberOfDigits);

                    // Finally, convert string to integer to show on the slot machine
                    number = int.Parse(num);
                }
            }
            catch (Exception)
            {
                // Who needs exception handling in a slot machine. It will work juuuust fine...
            }
        }
    }
}
