using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LuckyDeuceCasino
{
    public class OutOfTheWindowException : Exception
    {
        public OutOfTheWindowException(string message) : base(message)
        {

        }
    }
}
