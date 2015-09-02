using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace twdtf_craps.Classes {
	class CrapsRoller {
		#region probabilities
		private readonly decimal pushProbability = .0277M;
		private readonly decimal comeOutProbabilityMiddleGap = .1665M;
		private readonly decimal stayInProbability = .0833M; // probability of staying "in", from the low end
		#endregion

		private Random randSource;
		private List<int> _comeOutNumbers;
		private List<int> _comeOutDontPassNumbers;
		private List<int> _comeOutPushNumbers;
		private List<int> _comeOutMiddleGapNumbers;
		private List<DieEnumerator> dice;
		private Dictionary<int, decimal> odds;
		public CrapsRoller(int numDice, int faces) {
			randSource = new Random();

			dice = new List<DieEnumerator>();
			for (int die = 0; die < numDice; ++die) {
				dice.Add(new DieEnumerator(faces, randSource));
			}

			Setup();
		}

		private void Setup() {
			_maxRoll = dice.Select(d => d.faces).Sum();
			_averageRoll = dice.Select(d => d.average).Sum();
			

			GenerateOdds();

			_comeOutPushNumbers = new List<int>();
			_comeOutPushNumbers.Add(maxRoll);
			while (Probability(_comeOutPushNumbers) < pushProbability) {
				_comeOutPushNumbers.Add(_comeOutPushNumbers.Min() - 1);
			}

			_comeOutMiddleGapNumbers = new List<int>();
			_comeOutMiddleGapNumbers.Add((int)averageRoll);
			while (Probability(_comeOutMiddleGapNumbers) < comeOutProbabilityMiddleGap) {
				_comeOutMiddleGapNumbers.Add(_comeOutMiddleGapNumbers.Max() + 1);
				_comeOutMiddleGapNumbers.Add(_comeOutMiddleGapNumbers.Min() - 1);
			}

			_comeOutDontPassNumbers = new List<int>();
			_comeOutDontPassNumbers.Add(minRoll);
			while (Probability(_comeOutDontPassNumbers) < stayInProbability) {
				_comeOutDontPassNumbers.Add(_comeOutDontPassNumbers.Max() + 1);
			}

			_comeOutNumbers = new List<int>();
			for (int i = _comeOutDontPassNumbers.Max() + 1; i < _comeOutMiddleGapNumbers.Min(); ++i) {
				_comeOutNumbers.Add(i);
			}
			int numComeOutNumbers = _comeOutNumbers.Count;
			_comeOutNumbers.Add(_comeOutMiddleGapNumbers.Max() + 1);
			for (int i = 0; i < numComeOutNumbers - 1; ++i) {
				_comeOutNumbers.Add(_comeOutNumbers.Max() + 1);
			}

			BuildReport();
		}

		public void AddDie(int faces) {
			dice.Add(new DieEnumerator(faces, randSource));
			Setup();
		}

		public void AddDice(List<int> faceNums) {
			foreach (int face in faceNums) {
				dice.Add(new DieEnumerator(face, randSource));
			}

			Setup();
		}

		private void GenerateOdds() {
			// TODO: support reading in odds from a file (maybe "<numDice> <numFaces>.odds"?)
			var waysToGet = new Dictionary<int, int>();

			for (int i = 0; i <= maxRoll; ++i) {
				waysToGet[i] = 0;
			}

			do {
				waysToGet[SumDieEnumerators()]++;
				for (int i = 0; i < dice.Count; ++i) {
					dice[i].Next();
					if (!dice[i].overflow) {
						break;
					}
				}
			} while (!dice[dice.Count - 1].overflow);

			// debug: pseud-json-ize
			var sb = new StringBuilder();
			foreach (var key in waysToGet.Keys) {
				sb.AppendFormat("{0}:{1}", key, waysToGet[key]);
				sb.AppendLine();
			}
			var s = sb.ToString();
			// /debug

			decimal possibleResults = 0;
			foreach (var value in waysToGet.Values) {
				possibleResults += value;
			}
			odds = new Dictionary<int, decimal>();
			foreach (int i in waysToGet.Keys) {
				odds[i] = waysToGet[i] / possibleResults;
			}
		}

		private int SumDieEnumerators() {
			int sum = 0;
			foreach (var die in dice) {
				sum += die.current;
			}
			return sum;
		}

		private string _report;
		public string report { get { return _report; } }

		private void BuildReport() {
			var report = new StringBuilder();

			report.Append("Push on:");
			foreach (var val in _comeOutPushNumbers) {
				report.AppendFormat(" {0}", val);
			}
			report.AppendLine();
			report.Append("Come out on:");
			foreach (var val in _comeOutNumbers) {
				report.AppendFormat(" {0}", val);
			}
			report.AppendLine();
			report.Append("Middle Gap:");
			foreach (var val in _comeOutMiddleGapNumbers) {
				report.AppendFormat(" {0}", val);
			}
			report.AppendLine();
			report.Append("Pass on:");
			foreach (var val in _comeOutDontPassNumbers) {
				report.AppendFormat(" {0}", val);
			}
			report.AppendLine();
			report.AppendLine("Probabilities:");
			foreach (var key in odds.Keys) {
				report.AppendFormat("  {0} : {1}", key, odds[key]);
				report.AppendLine();
			}

			_report = report.ToString();
		}

		public decimal Probability(List<int> values) {
			decimal prob = 0;
			foreach (var val in values) {
				prob += odds[val];
			}
			return prob;
		}

		/// <summary>
		/// Roll the dice. Update the state based on the roll.
		/// </summary>
		/// <returns></returns>
		public int Roll() {
			int result = 0;
			foreach (var die in dice) {
				result += die.Roll();
			}
			return result;
		}

		public int minRoll { get { return dice.Count; } }

		public int maxRoll { get { return _maxRoll; } }
		private int _maxRoll;

		public decimal averageRoll { get { return _averageRoll; } }
		private decimal _averageRoll;

		/// <summary>
		/// Does the roll "come out"?
		/// </summary>
		/// <param name="roll"></param>
		/// <returns></returns>
		public bool ComesOut(int roll) {
			return _comeOutNumbers.Contains(roll);
		}

		/// <summary>
		/// Does a Pass bet win?
		/// </summary>
		/// <param name="roll"></param>
		/// <returns></returns>
		public bool PassBetResult(int roll) {
			return !_comeOutDontPassNumbers.Contains(roll) && !_comeOutNumbers.Contains(roll);
		}

		/// <summary>
		/// How does a Don't Pass bet fare?
		/// </summary>
		/// <param name="roll"></param>
		/// <returns>Wins (true), Loses (false), Pushes (null)</returns>
		public bool? DontPassBetResult(int roll) {
			if (_comeOutNumbers.Contains(roll)) {
				return false;
			}
			if (_comeOutPushNumbers.Contains(roll)) {
				return null;
			}
			return !PassBetResult(roll);
		}
	}
}
