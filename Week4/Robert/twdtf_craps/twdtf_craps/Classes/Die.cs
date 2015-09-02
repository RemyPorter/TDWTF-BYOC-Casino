using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace twdtf_craps.Classes {
	class Die {
		public Die(int faces, Random randSource) {
			_faces = faces;
			_randMax = +faces + 1;
			_source = randSource;
			_average = (((decimal)faces + 1) / 2);
		}

		private Random _source;

		private int _randMax;
		private int _faces;
		public int faces { get { return _faces; } }

		private decimal _average;
		public decimal average { get { return _average; } }

		public int Roll() {
			return _source.Next(1, _randMax);
		}
	}
}
