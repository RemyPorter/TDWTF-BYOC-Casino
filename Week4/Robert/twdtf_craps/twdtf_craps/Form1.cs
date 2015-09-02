using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace twdtf_craps {
	public partial class Form1 : Form {
		public Form1() {
			InitializeComponent();
			numDice.Maximum = decimal.MaxValue;
			numFaces.Maximum = decimal.MaxValue;
		}

		private Classes.CrapsRoller roller;

		private void buttonReset_Click(object sender, EventArgs e) {
			// it can start taking a _long_ time to caluculate odds for small values of "large" numbers
			if (numDice.Value * numFaces.Value >= 70) {
				var response = MessageBox.Show("Warning: this may take a while to calculate. Continue?", "This may take a while...", MessageBoxButtons.YesNo);
				if (response == DialogResult.No) {
					return;
				}
			}
			var timer = new System.Diagnostics.Stopwatch();
			timer.Start();
			roller = new Classes.CrapsRoller(numDice: (int)numDice.Value, faces: (int)numFaces.Value);
			timer.Stop();
			textBoxResult.Text = string.Format("Calculating odds took {0}.", timer.Elapsed.ToString());

			buttonRoll.Enabled = true;
			buttonAddDice.Enabled = true;
			textBoxAddDice.Enabled = true;
			buttonDiagnostics.Enabled = true;
			_lastRoll = roller.minRoll;
		}

		private string DebugOutput() {
			var message = new StringBuilder();

			message.AppendLine("roll : pass : don't : comes out");
			for (int roll = roller.minRoll; roll <= roller.maxRoll; ++roll) {
				message.AppendFormat("{0}: {1} {2} {3}{4}", roll, roller.PassBetResult(roll), roller.DontPassBetResult(roll) == null ? "NULL" : roller.DontPassBetResult(roll).ToString(), roller.ComesOut(roll), Environment.NewLine);
			}
			message.AppendLine();
			message.AppendLine(roller.report);

			return message.ToString();
		}

		private int _lastRoll;

		private void buttonRoll_Click(object sender, EventArgs e) {
			var result = new StringBuilder();
			int roll = roller.Roll();
			//int roll = _lastRoll;
			//_lastRoll++;
			bool passResult = roller.PassBetResult(roll);
			string passBetResult = passResult ? "Wins" : "Loses";
			string dontPassBetResult;
			bool? dontResult = roller.DontPassBetResult(roll);
			if (dontResult == null) {
				dontPassBetResult = "Push";
			} else {
				dontPassBetResult = (bool)dontResult ? "Wins" : "Loses";
			}
			bool cameOut = roller.ComesOut(roll);
			string passBetWinner = cameOut ? "Neither (came out)" : dontResult == null ? "Neither" : passResult ? "Pass" : "Don't Pass";

			result.AppendFormat("Roll: {0}", roll);
			result.AppendLine();
			result.AppendFormat("Came out: {0}", cameOut);
			result.AppendLine();
			result.AppendFormat("Pass Bet: {0}", passBetResult);
			result.AppendLine();
			result.AppendFormat("Don't Pass Bet: {0}", dontPassBetResult);
			result.AppendLine();
			result.AppendFormat("Pass Bet Winner: {0}", passBetWinner);


			textBoxResult.Text = result.ToString();
		}

		private void buttonRand_Click(object sender, EventArgs e) {
			var rand = new Random();
			// Out Of Memory exceptions get thrown if these numbers get too big
			numDice.Value = rand.Next(1, 1001);
			numFaces.Value = rand.Next(2, 1001);
		}

		private void buttonAddDice_Click(object sender, EventArgs e) {
			var faces = new List<int>();

			foreach (var num in textBoxAddDice.Text.Split()) {
				int facesToAdd;
				if (int.TryParse(num, out facesToAdd)) {
					if (facesToAdd < 2) {
						MessageBox.Show("Error: cannot add a die with " + facesToAdd + " faces; minimum of 2.");
					}
					faces.Add(facesToAdd);
				} else {
					MessageBox.Show("Could not parse '" + num + "' to an int.");
					return;
				}
			}

			roller.AddDice(faces);
			textBoxAddDice.Text = "";
		}

		private void buttonDiagnostics_Click(object sender, EventArgs e) {
			System.IO.File.WriteAllText("debug.txt", DebugOutput());
		}
	}
}
