using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LuckyDeuceCasino
{
    class Program
    {
        static List<string> _playerNames = new List<string> {
            {"Bob"},{"MAlice"},{"Rob"},{"Mark"},{"Alex"}
        };
        protected internal static string cp = string.Empty;
        static void Main(string[] args)
        {
            CardPool.NumberOfDecks = 5;
            CardPool deck = new CardPool();
            while (true)
            {
                foreach(string player in _playerNames)
                {
                    cp = player;
                    Console.WriteLine("Drawing hand");
                    var hand = deck.DrawHand(5);
                    Console.Write(player + "\t");
                    foreach (var card in hand)
                    {
                        Console.Write(card.Color.GetDisplayName() + card.Value.GetDisplayName());
                    }
                    Console.WriteLine();
                }
                Console.WriteLine("Press any key to draw next hands");

                Console.ReadKey();
            }
        }
    }
}
