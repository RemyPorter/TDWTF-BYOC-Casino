namespace twdtf_craps {
	partial class Form1 {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.label1 = new System.Windows.Forms.Label();
			this.numDice = new System.Windows.Forms.NumericUpDown();
			this.numFaces = new System.Windows.Forms.NumericUpDown();
			this.label2 = new System.Windows.Forms.Label();
			this.buttonRoll = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.buttonReset = new System.Windows.Forms.Button();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.textBoxResult = new System.Windows.Forms.TextBox();
			this.buttonRand = new System.Windows.Forms.Button();
			this.textBoxAddDice = new System.Windows.Forms.TextBox();
			this.buttonAddDice = new System.Windows.Forms.Button();
			this.buttonDiagnostics = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.numDice)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numFaces)).BeginInit();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 21);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(84, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Number of Dice:";
			// 
			// numDice
			// 
			this.numDice.Location = new System.Drawing.Point(96, 19);
			this.numDice.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
			this.numDice.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numDice.Name = "numDice";
			this.numDice.Size = new System.Drawing.Size(57, 20);
			this.numDice.TabIndex = 1;
			this.numDice.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
			// 
			// numFaces
			// 
			this.numFaces.Location = new System.Drawing.Point(96, 45);
			this.numFaces.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
			this.numFaces.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
			this.numFaces.Name = "numFaces";
			this.numFaces.Size = new System.Drawing.Size(57, 20);
			this.numFaces.TabIndex = 2;
			this.numFaces.Value = new decimal(new int[] {
            6,
            0,
            0,
            0});
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(6, 47);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(77, 13);
			this.label2.TabIndex = 3;
			this.label2.Text = "Faces Per Die:";
			// 
			// buttonRoll
			// 
			this.buttonRoll.Enabled = false;
			this.buttonRoll.Location = new System.Drawing.Point(12, 235);
			this.buttonRoll.Name = "buttonRoll";
			this.buttonRoll.Size = new System.Drawing.Size(75, 23);
			this.buttonRoll.TabIndex = 4;
			this.buttonRoll.Text = "Roll!";
			this.buttonRoll.UseVisualStyleBackColor = true;
			this.buttonRoll.Click += new System.EventHandler(this.buttonRoll_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.buttonRand);
			this.groupBox1.Controls.Add(this.buttonReset);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.numDice);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.numFaces);
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(161, 111);
			this.groupBox1.TabIndex = 5;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Die Settings";
			// 
			// buttonReset
			// 
			this.buttonReset.Location = new System.Drawing.Point(9, 71);
			this.buttonReset.Name = "buttonReset";
			this.buttonReset.Size = new System.Drawing.Size(81, 23);
			this.buttonReset.TabIndex = 4;
			this.buttonReset.Text = "Start/Restart";
			this.buttonReset.UseVisualStyleBackColor = true;
			this.buttonReset.Click += new System.EventHandler(this.buttonReset_Click);
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.label3);
			this.groupBox2.Controls.Add(this.buttonAddDice);
			this.groupBox2.Controls.Add(this.textBoxAddDice);
			this.groupBox2.Location = new System.Drawing.Point(179, 12);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(174, 111);
			this.groupBox2.TabIndex = 6;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Add Dice";
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.textBoxResult);
			this.groupBox4.Location = new System.Drawing.Point(12, 129);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(341, 100);
			this.groupBox4.TabIndex = 0;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Result";
			// 
			// textBoxResult
			// 
			this.textBoxResult.Location = new System.Drawing.Point(6, 19);
			this.textBoxResult.Multiline = true;
			this.textBoxResult.Name = "textBoxResult";
			this.textBoxResult.ReadOnly = true;
			this.textBoxResult.Size = new System.Drawing.Size(329, 75);
			this.textBoxResult.TabIndex = 0;
			// 
			// buttonRand
			// 
			this.buttonRand.Location = new System.Drawing.Point(96, 71);
			this.buttonRand.Name = "buttonRand";
			this.buttonRand.Size = new System.Drawing.Size(57, 23);
			this.buttonRand.TabIndex = 5;
			this.buttonRand.Text = "Rand!";
			this.buttonRand.UseVisualStyleBackColor = true;
			this.buttonRand.Click += new System.EventHandler(this.buttonRand_Click);
			// 
			// textBoxAddDice
			// 
			this.textBoxAddDice.Enabled = false;
			this.textBoxAddDice.Location = new System.Drawing.Point(6, 32);
			this.textBoxAddDice.Name = "textBoxAddDice";
			this.textBoxAddDice.Size = new System.Drawing.Size(162, 20);
			this.textBoxAddDice.TabIndex = 0;
			// 
			// buttonAddDice
			// 
			this.buttonAddDice.Enabled = false;
			this.buttonAddDice.Location = new System.Drawing.Point(6, 58);
			this.buttonAddDice.Name = "buttonAddDice";
			this.buttonAddDice.Size = new System.Drawing.Size(162, 23);
			this.buttonAddDice.TabIndex = 1;
			this.buttonAddDice.Text = "Add Di(c)e!";
			this.buttonAddDice.UseVisualStyleBackColor = true;
			this.buttonAddDice.Click += new System.EventHandler(this.buttonAddDice_Click);
			// 
			// buttonDiagnostics
			// 
			this.buttonDiagnostics.Enabled = false;
			this.buttonDiagnostics.Location = new System.Drawing.Point(272, 235);
			this.buttonDiagnostics.Name = "buttonDiagnostics";
			this.buttonDiagnostics.Size = new System.Drawing.Size(75, 23);
			this.buttonDiagnostics.TabIndex = 2;
			this.buttonDiagnostics.Text = "Diagnostics";
			this.buttonDiagnostics.UseVisualStyleBackColor = true;
			this.buttonDiagnostics.Click += new System.EventHandler(this.buttonDiagnostics_Click);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(3, 16);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(164, 13);
			this.label3.TabIndex = 2;
			this.label3.Text = "Add a space-delimited list of dice:";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(365, 271);
			this.Controls.Add(this.buttonDiagnostics);
			this.Controls.Add(this.groupBox4);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.buttonRoll);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Name = "Form1";
			this.Text = "Form1";
			((System.ComponentModel.ISupportInitialize)(this.numDice)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numFaces)).EndInit();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.groupBox4.ResumeLayout(false);
			this.groupBox4.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.NumericUpDown numDice;
		private System.Windows.Forms.NumericUpDown numFaces;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button buttonRoll;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.TextBox textBoxResult;
		private System.Windows.Forms.Button buttonReset;
		private System.Windows.Forms.Button buttonRand;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Button buttonAddDice;
		private System.Windows.Forms.TextBox textBoxAddDice;
		private System.Windows.Forms.Button buttonDiagnostics;
	}
}

