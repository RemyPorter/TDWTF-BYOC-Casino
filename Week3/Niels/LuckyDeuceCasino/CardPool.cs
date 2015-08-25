using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LuckyDeuceCasino
{
    public class CardPool
    {
        public static int NumberOfDecks { get; set; }
        protected internal int cardIndex = 0;

        List<Tuple<int, CardColor, CardValue>> cards;
        public static bool checkHand = false;
        public CardPool()
        {
            cards = new List<Tuple<int, CardColor, CardValue>>();
            for (int i = 0; i < NumberOfDecks; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 1; k <= 13; k++)
                    {
                        cards.Add(new Tuple<int, CardColor, CardValue>(i,(CardColor)j,(CardValue)k));
                    }
                }
            }
            cards.Shuffle();
        }

        public Card DrawCard()
        {
            if (cardIndex >= cards.Count)
            {
                cardIndex = 0;
                cards.Shuffle();
            }
            var cardData = cards[cardIndex];
            cardIndex = cardIndex+1;
            return new Card { Color = cardData.Item2, Value = cardData.Item3 };
        }

        public List<Card> DrawHand(int numberOfCards)
        {
            int v = cards.Take(cardIndex).Select(c => c.Item3.ToInt()).Sum();
            //Console.WriteLine("V: {0}", v);
            //check on entropy
            if (v / (2<<3) > 0)
            {
                //There are already identitcal hands played, check if next hand is different
                checkHand = true;
            }
            List<Card> hand = new List<Card>(numberOfCards);
            for (int i = 0; i < numberOfCards; i++)
            {
                hand.Add(DrawCard());
            }
            if (checkHand)
            {
                //will throw exception to terminate. Ensures that no suspicious hands are drawn
                hand.ForEach(c => c.CheckValue());
            }
            return hand;
        }
    }
}
