using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace twdtf_craps.Classes {
	class DieEnumerator : Die{
		public DieEnumerator(int faces, Random randSource) : base(faces, randSource) {
			_current = 1;
			_overflow = false;
		}

		public int current { get { return _current; } }
		private int _current;

		public bool overflow { get { return _overflow; } }
		private bool _overflow;

		public void Next() {
			_overflow = false;
			++_current;
			if (_current == faces + 1) {
				_current = 1;
				_overflow = true;
			}
		}
	}
}
