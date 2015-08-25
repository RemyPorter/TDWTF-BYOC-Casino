using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;

namespace LuckyDeuceCasino
{
    public enum CardValue
    {
        [Display(Name = "A")]
        Ace = 1,
        [Display(Name = "2")]
        Two = 2,
        [Display(Name = "3")]
        Three = 3,
        [Display(Name = "4")]
        Four = 4,
        [Display(Name = "5")]
        Five = 5,
        [Display(Name = "6")]
        Six = 6,
        [Display(Name = "7")]
        Seven = 7,
        [Display(Name = "8")]
        Eight = 8,
        [Display(Name = "9")]
        Nine = 9,
        [Display(Name = "10")]
        Ten = 10,
        [Display(Name = "J")]
        Jack = 11,
        [Display(Name = "Q")]
        Queen = 12,
        [Display(Name = "K")]
        King = 13
    }
}
