using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;

namespace LuckyDeuceCasino
{
    public enum CardColor
    {
        [Display(Name = "♥")]
        Hearts = 0,
        [Display(Name = "♠")]
        Spades = 1,
        [Display(Name = "♣")]
        Clubs = 2,
        [Display(Name = "♦")]
        Diamonds = 3
    }
}
