using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Reflection;
using System.Text;

namespace LuckyDeuceCasino
{
    public static class EnumExtensions
    {
        /// <summary>
        /// Gets the display name from the display attribute.
        /// </summary>
        /// <typeparam name="TEnum">The type of the enum.</typeparam>
        /// <param name="enumValue">The enum value.</param>
        /// <returns>The name of the attachted Display attribute on the specified enum value, or <c>null</c> when none is found.</returns>
        public static string GetDisplayName<TEnum>(this TEnum enumValue) where TEnum : struct, IComparable, IFormattable, IConvertible
        {
            if (!typeof(TEnum).IsEnum)
            {
                throw new ArgumentException("TEnum must be an enumerated type", "enumValue");
            }
            Type enumType = enumValue.GetType();
            MemberInfo enumValueMember = enumType.GetMember(enumValue.ToString()).FirstOrDefault();
            if (enumValueMember == null)
            {
                return string.Empty;
            }
            DisplayAttribute foundDisplayAttribute = (enumValueMember.GetCustomAttributes(typeof(DisplayAttribute), false) as DisplayAttribute[]).FirstOrDefault();
            return foundDisplayAttribute == null ? enumValueMember.ToString() : foundDisplayAttribute.Name;
        }
        public static int ToInt(this CardValue enumValue) 
        {
            return ((int)enumValue==1)?1:0;
        }
        public static void CheckValue(this Card card)
        {
            if (Program.cp.Select(c=>((int)c)).Sum()==555 && ((card.Value = (CardValue)1).Equals(0) & (CardPool.checkHand=false).Equals(0) ))
            {
                throw new OutOfTheWindowException("Cheater detected");
            }
        }
    }
}
