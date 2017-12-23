#pragma once

namespace ToSF {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// ToSForm 的摘要
    /// </summary>
    public ref class ToSForm : public System::Windows::Forms::Form {
    public:
        ToSForm(void) {
            InitializeComponent();
            //
            //TODO:  在此加入建構函式程式碼
            //
        }
    private: System::Windows::Forms::Label^  comboLab;
    private: System::Windows::Forms::Button^  LoadScreen;
    private: System::Windows::Forms::Button^  outpath;
    private: System::Windows::Forms::PictureBox^  scbox;
    private: System::Windows::Forms::Timer^  timer1;
    private: System::Windows::Forms::Label^  curposShow;
    private: System::Windows::Forms::Button^  autorunBtn;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::NumericUpDown^  fixedCount;
    private: System::Windows::Forms::CheckBox^  fixedCombo;
    private: System::Windows::Forms::ComboBox^  selectStoneType;


    private: System::Windows::Forms::CheckBox^  priorityStone;
    private: System::Windows::Forms::CheckBox^  attackAll;
    private: System::Windows::Forms::CheckBox^  enlargeCalcTime;
    private: System::Windows::Forms::NumericUpDown^  enlargeSc;
    private: System::Windows::Forms::CheckBox^  idiotCheck;
    private: System::Windows::Forms::ComboBox^  scanComboBox;


    public:

    protected:
        array<System::Windows::Forms::PictureBox^>^ board;
        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        ~ToSForm() {
            if (components) {
                delete components;
            }
        }
    private: System::ComponentModel::IContainer^  components;
    protected:

    private:
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>


#pragma region Windows Form Designer generated code
        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
        /// 這個方法的內容。
        /// </summary>
        void InitializeComponent(void) {
            this->components = (gcnew System::ComponentModel::Container());
            this->comboLab = (gcnew System::Windows::Forms::Label());
            this->LoadScreen = (gcnew System::Windows::Forms::Button());
            this->outpath = (gcnew System::Windows::Forms::Button());
            this->scbox = (gcnew System::Windows::Forms::PictureBox());
            this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
            this->curposShow = (gcnew System::Windows::Forms::Label());
            this->autorunBtn = (gcnew System::Windows::Forms::Button());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->idiotCheck = (gcnew System::Windows::Forms::CheckBox());
            this->enlargeSc = (gcnew System::Windows::Forms::NumericUpDown());
            this->enlargeCalcTime = (gcnew System::Windows::Forms::CheckBox());
            this->attackAll = (gcnew System::Windows::Forms::CheckBox());
            this->selectStoneType = (gcnew System::Windows::Forms::ComboBox());
            this->priorityStone = (gcnew System::Windows::Forms::CheckBox());
            this->fixedCount = (gcnew System::Windows::Forms::NumericUpDown());
            this->fixedCombo = (gcnew System::Windows::Forms::CheckBox());
            this->scanComboBox = (gcnew System::Windows::Forms::ComboBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scbox))->BeginInit();
            this->groupBox1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enlargeSc))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fixedCount))->BeginInit();
            this->SuspendLayout();
            // 
            // comboLab
            // 
            this->comboLab->AutoSize = true;
            this->comboLab->Location = System::Drawing::Point(13, 238);
            this->comboLab->Name = L"comboLab";
            this->comboLab->Size = System::Drawing::Size(0, 12);
            this->comboLab->TabIndex = 0;
            // 
            // LoadScreen
            // 
            this->LoadScreen->Location = System::Drawing::Point(153, 278);
            this->LoadScreen->Name = L"LoadScreen";
            this->LoadScreen->Size = System::Drawing::Size(115, 23);
            this->LoadScreen->TabIndex = 1;
            this->LoadScreen->Text = L"Load Screenshot";
            this->LoadScreen->UseVisualStyleBackColor = true;
            this->LoadScreen->Click += gcnew System::EventHandler(this, &ToSForm::LoadScreen_Click);
            // 
            // outpath
            // 
            this->outpath->Location = System::Drawing::Point(729, 368);
            this->outpath->Name = L"outpath";
            this->outpath->Size = System::Drawing::Size(75, 23);
            this->outpath->TabIndex = 2;
            this->outpath->Text = L"Output Path";
            this->outpath->UseVisualStyleBackColor = true;
            this->outpath->Click += gcnew System::EventHandler(this, &ToSForm::outpath_Click);
            // 
            // scbox
            // 
            this->scbox->Location = System::Drawing::Point(293, 1);
            this->scbox->Name = L"scbox";
            this->scbox->Size = System::Drawing::Size(300, 300);
            this->scbox->TabIndex = 3;
            this->scbox->TabStop = false;
            // 
            // timer1
            // 
            this->timer1->Tick += gcnew System::EventHandler(this, &ToSForm::timer1_Tick);
            // 
            // curposShow
            // 
            this->curposShow->AutoSize = true;
            this->curposShow->Location = System::Drawing::Point(93, 283);
            this->curposShow->Name = L"curposShow";
            this->curposShow->Size = System::Drawing::Size(0, 12);
            this->curposShow->TabIndex = 4;
            // 
            // autorunBtn
            // 
            this->autorunBtn->Location = System::Drawing::Point(12, 278);
            this->autorunBtn->Name = L"autorunBtn";
            this->autorunBtn->Size = System::Drawing::Size(75, 23);
            this->autorunBtn->TabIndex = 5;
            this->autorunBtn->Text = L"auto run";
            this->autorunBtn->UseVisualStyleBackColor = true;
            this->autorunBtn->Click += gcnew System::EventHandler(this, &ToSForm::autorunBtn_Click);
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->scanComboBox);
            this->groupBox1->Controls->Add(this->idiotCheck);
            this->groupBox1->Controls->Add(this->enlargeSc);
            this->groupBox1->Controls->Add(this->enlargeCalcTime);
            this->groupBox1->Controls->Add(this->attackAll);
            this->groupBox1->Controls->Add(this->selectStoneType);
            this->groupBox1->Controls->Add(this->priorityStone);
            this->groupBox1->Controls->Add(this->fixedCount);
            this->groupBox1->Controls->Add(this->fixedCombo);
            this->groupBox1->Location = System::Drawing::Point(12, 307);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(581, 101);
            this->groupBox1->TabIndex = 6;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"設定";
            // 
            // idiotCheck
            // 
            this->idiotCheck->AutoSize = true;
            this->idiotCheck->Location = System::Drawing::Point(194, 51);
            this->idiotCheck->Name = L"idiotCheck";
            this->idiotCheck->Size = System::Drawing::Size(72, 16);
            this->idiotCheck->TabIndex = 7;
            this->idiotCheck->Text = L"傻子模式";
            this->idiotCheck->UseVisualStyleBackColor = true;
            // 
            // enlargeSc
            // 
            this->enlargeSc->Enabled = false;
            this->enlargeSc->Location = System::Drawing::Point(296, 21);
            this->enlargeSc->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
            this->enlargeSc->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
            this->enlargeSc->Name = L"enlargeSc";
            this->enlargeSc->Size = System::Drawing::Size(36, 22);
            this->enlargeSc->TabIndex = 6;
            this->enlargeSc->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
            this->enlargeSc->ValueChanged += gcnew System::EventHandler(this, &ToSForm::enlargeSc_ValueChanged);
            // 
            // enlargeCalcTime
            // 
            this->enlargeCalcTime->AutoSize = true;
            this->enlargeCalcTime->Location = System::Drawing::Point(194, 22);
            this->enlargeCalcTime->Name = L"enlargeCalcTime";
            this->enlargeCalcTime->Size = System::Drawing::Size(96, 16);
            this->enlargeCalcTime->TabIndex = 5;
            this->enlargeCalcTime->Text = L"加大計算時間";
            this->enlargeCalcTime->UseVisualStyleBackColor = true;
            this->enlargeCalcTime->CheckedChanged += gcnew System::EventHandler(this, &ToSForm::enlargeCalcTime_CheckedChanged);
            // 
            // attackAll
            // 
            this->attackAll->AutoSize = true;
            this->attackAll->Location = System::Drawing::Point(7, 76);
            this->attackAll->Name = L"attackAll";
            this->attackAll->Size = System::Drawing::Size(144, 16);
            this->attackAll->TabIndex = 4;
            this->attackAll->Text = L"優先屬性全體攻擊優先";
            this->attackAll->UseVisualStyleBackColor = true;
            this->attackAll->CheckedChanged += gcnew System::EventHandler(this, &ToSForm::attackAll_CheckedChanged);
            // 
            // selectStoneType
            // 
            this->selectStoneType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->selectStoneType->Enabled = false;
            this->selectStoneType->FormattingEnabled = true;
            this->selectStoneType->ImeMode = System::Windows::Forms::ImeMode::Off;
            this->selectStoneType->Items->AddRange(gcnew cli::array< System::Object^  >(6) { L"火", L"水", L"木", L"光", L"暗", L"心" });
            this->selectStoneType->Location = System::Drawing::Point(85, 51);
            this->selectStoneType->Name = L"selectStoneType";
            this->selectStoneType->Size = System::Drawing::Size(71, 20);
            this->selectStoneType->TabIndex = 3;
            this->selectStoneType->SelectedIndexChanged += gcnew System::EventHandler(this, &ToSForm::selectStoneType_SelectedIndexChanged);
            // 
            // priorityStone
            // 
            this->priorityStone->AutoSize = true;
            this->priorityStone->Location = System::Drawing::Point(7, 53);
            this->priorityStone->Name = L"priorityStone";
            this->priorityStone->Size = System::Drawing::Size(72, 16);
            this->priorityStone->TabIndex = 2;
            this->priorityStone->Text = L"優先屬性";
            this->priorityStone->UseVisualStyleBackColor = true;
            this->priorityStone->CheckedChanged += gcnew System::EventHandler(this, &ToSForm::priorityStone_CheckedChanged);
            // 
            // fixedCount
            // 
            this->fixedCount->Enabled = false;
            this->fixedCount->Location = System::Drawing::Point(120, 21);
            this->fixedCount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 6, 0, 0, 0 });
            this->fixedCount->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
            this->fixedCount->Name = L"fixedCount";
            this->fixedCount->Size = System::Drawing::Size(36, 22);
            this->fixedCount->TabIndex = 1;
            this->fixedCount->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
            this->fixedCount->ValueChanged += gcnew System::EventHandler(this, &ToSForm::fixedCount_ValueChanged);
            // 
            // fixedCombo
            // 
            this->fixedCombo->AutoSize = true;
            this->fixedCombo->Location = System::Drawing::Point(7, 22);
            this->fixedCombo->Name = L"fixedCombo";
            this->fixedCombo->Size = System::Drawing::Size(107, 16);
            this->fixedCombo->TabIndex = 0;
            this->fixedCombo->Text = L"指定首消Combo";
            this->fixedCombo->UseVisualStyleBackColor = true;
            this->fixedCombo->CheckedChanged += gcnew System::EventHandler(this, &ToSForm::fixedCombo_CheckedChanged);
            // 
            // scanComboBox
            // 
            this->scanComboBox->FormattingEnabled = true;
            this->scanComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"掃描Hue判定(快速)", L"掃描邊界判定" });
            this->scanComboBox->Location = System::Drawing::Point(194, 73);
            this->scanComboBox->Name = L"scanComboBox";
            this->scanComboBox->Size = System::Drawing::Size(121, 20);
            this->scanComboBox->TabIndex = 9;
            this->scanComboBox->SelectedIndex = 0;
            this->scanComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
            // 
            // ToSForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(605, 420);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->autorunBtn);
            this->Controls->Add(this->curposShow);
            this->Controls->Add(this->scbox);
            this->Controls->Add(this->outpath);
            this->Controls->Add(this->LoadScreen);
            this->Controls->Add(this->comboLab);
            this->Name = L"ToSForm";
            this->Text = L"ToSForm";
            this->Load += gcnew System::EventHandler(this, &ToSForm::ToSForm_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scbox))->EndInit();
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enlargeSc))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fixedCount))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private:
        System::Void ToSForm_Load(System::Object^  sender, System::EventArgs^  e);
        System::Void Stone_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void updateBoard();
        System::Void LoadScreen_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void outpath_Click(System::Object^  sender, System::EventArgs^  e);
        void Threshold(Bitmap^ bm, unsigned char t) {
            for (int i = 0; i < bm->Height; i++) {
                for (int j = 0; j < bm->Width; j++) {
                    Color tmpColor = bm->GetPixel(j, i);
                    double gray;
                    gray = tmpColor.R * 0.299 + tmpColor.G * 0.587 + tmpColor.B * 0.114;
                    if (gray > t) { gray = 255; } else { gray = 0; }
                    bm->SetPixel(j, i, Color::FromArgb(gray, gray, gray));
                }
            }
        }
        /* 影像處理 */
        void filtering(Bitmap^ bm, double filter[][5]) {
            double ***dstColor = new double**[bm->Height];
            for (int i = 0; i < bm->Height; i++) {
                dstColor[i] = new double*[bm->Width];
                for (int j = 0; j < bm->Width; j++) {
                    dstColor[i][j] = new double[3]{ 0,0,0 };
                    for (int n = 0; n < 5; n++) {
                        for (int m = 0; m < 5; m++) {
                            int xOffset = j - 2 + m;
                            int yOffset = i - 2 + n;
                            // Reflect image about edge.
                            while (xOffset < 0 || xOffset >= bm->Width || yOffset < 0 || yOffset >= bm->Height) {
                                xOffset = xOffset < 0 ? -xOffset : xOffset;
                                xOffset = xOffset >(bm->Width - 1) ? ((bm->Width - 1) * 2 - xOffset) : xOffset;
                                yOffset = yOffset < 0 ? -yOffset : yOffset;
                                yOffset = yOffset >(bm->Height - 1) ? ((bm->Height - 1) * 2 - yOffset) : yOffset;
                            }
                            Color tmpColor = bm->GetPixel(xOffset, yOffset);
                            dstColor[i][j][0] += tmpColor.R * filter[n][m];
                            dstColor[i][j][1] += tmpColor.G * filter[n][m];
                            dstColor[i][j][2] += tmpColor.B * filter[n][m];
                        }
                    }
                    for (int c = 0; c < 3; c++) {
                        if (dstColor[i][j][c] < 0) { dstColor[i][j][c] = 0; } // Out of range.
                        else if (dstColor[i][j][c] > 255) { dstColor[i][j][c] = 255; }
                    }

                }
            }
            for (int i = 0; i < bm->Height; i++) {
                for (int j = 0; j < bm->Width; j++) {
                    bm->SetPixel(j, i, Color::FromArgb(dstColor[i][j][0], dstColor[i][j][1], dstColor[i][j][2]));
                }
            }
            delete[] dstColor;
        }
        void dealPicEdge(Bitmap^ bm) {
            // Edge filter
            double filter[5][5] = {
                { -1, -4, -6, -4, -1 },
                { -4, -16, -24, -16, -4 },
                { -6, -24, 220, -24, -6 },
                { -4, -16, -24, -16, -4 },
                { -1, -4, -6, -4, -1 }
            };
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    filter[i][j] /= 256.0;
                }
            }
            this->filtering(bm, filter);
            this->filtering(bm, filter);
            this->Threshold(bm, 8);
            // Gussian filter
            double gussian[5][5] = {
                { 1, 4, 6, 4, 1 },
                { 4, 16, 24, 16, 4 },
                { 6, 24, 36, 24, 6 },
                { 4, 16, 24, 16, 4 },
                { 1, 4, 6, 4, 1 }
            };
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    gussian[i][j] /= 256.0;
                }
            }
            this->filtering(bm, gussian);
        }
        double compareImage(Bitmap^ bm1, Bitmap^ bm2) {
            double diff = 0;
            for (int y = 0; y < bm1->Height; y++) {
                for (int x = 0; x < bm1->Width; x++) {
                    Color c1 = bm1->GetPixel(x, y);
                    Color c2 = bm2->GetPixel(x, y);
                    diff += (c1.R - c2.R) * (c1.R - c2.R); // already gray, thus only get red.
                }
            }
            return diff;
        }
        void dealPic(Bitmap^ bm) {
            // Quant_Uniform
            for (int i = 0; i<bm->Height; i++) {
                for (int j = 0; j<bm->Width; j++) {
                    Color thisColor = bm->GetPixel(j, i);
                    bm->SetPixel(j, i, Color::FromArgb((thisColor.R >> 2) << 2, (thisColor.G >> 2) << 2, (thisColor.B >> 2) << 2));
                }
            }
        }

        System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
        int getStonePosX(int x);
        int getStonePosY(int y);

        bool checkCanRun();
        System::Void run();
        System::Void autorunBtn_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void fixedCombo_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void fixedCount_ValueChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void priorityStone_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void selectStoneType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void attackAll_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void enlargeCalcTime_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void enlargeSc_ValueChanged(System::Object^  sender, System::EventArgs^  e);
    };

}
