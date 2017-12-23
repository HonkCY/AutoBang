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
    private: System::Windows::Forms::Button^  calcPathBtn;


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
    private: System::Windows::Forms::CheckBox^  AutoRunChk;
    private: System::Windows::Forms::GroupBox^  screenGroup;










    public:

    protected:
        array<System::Windows::Forms::PictureBox^>^ board;
    private: System::Windows::Forms::Button^  RunPathBtn;
    private: System::Windows::Forms::ListBox^  pathList;
    private: System::Windows::Forms::GroupBox^  pathGroup;
    private: System::Windows::Forms::Button^  addPathBtn;
    private: System::Windows::Forms::Button^  removePathBtn;
    private: System::Windows::Forms::Button^  clearPathBtn;
    protected:
        array<System::Windows::Forms::PictureBox^>^ finalBoard;
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
            this->calcPathBtn = (gcnew System::Windows::Forms::Button());
            this->scbox = (gcnew System::Windows::Forms::PictureBox());
            this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
            this->curposShow = (gcnew System::Windows::Forms::Label());
            this->autorunBtn = (gcnew System::Windows::Forms::Button());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->scanComboBox = (gcnew System::Windows::Forms::ComboBox());
            this->idiotCheck = (gcnew System::Windows::Forms::CheckBox());
            this->enlargeSc = (gcnew System::Windows::Forms::NumericUpDown());
            this->enlargeCalcTime = (gcnew System::Windows::Forms::CheckBox());
            this->attackAll = (gcnew System::Windows::Forms::CheckBox());
            this->selectStoneType = (gcnew System::Windows::Forms::ComboBox());
            this->priorityStone = (gcnew System::Windows::Forms::CheckBox());
            this->fixedCount = (gcnew System::Windows::Forms::NumericUpDown());
            this->fixedCombo = (gcnew System::Windows::Forms::CheckBox());
            this->AutoRunChk = (gcnew System::Windows::Forms::CheckBox());
            this->screenGroup = (gcnew System::Windows::Forms::GroupBox());
            this->RunPathBtn = (gcnew System::Windows::Forms::Button());
            this->pathList = (gcnew System::Windows::Forms::ListBox());
            this->pathGroup = (gcnew System::Windows::Forms::GroupBox());
            this->removePathBtn = (gcnew System::Windows::Forms::Button());
            this->addPathBtn = (gcnew System::Windows::Forms::Button());
            this->clearPathBtn = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scbox))->BeginInit();
            this->groupBox1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enlargeSc))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fixedCount))->BeginInit();
            this->screenGroup->SuspendLayout();
            this->pathGroup->SuspendLayout();
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
            this->LoadScreen->Location = System::Drawing::Point(969, 256);
            this->LoadScreen->Name = L"LoadScreen";
            this->LoadScreen->Size = System::Drawing::Size(94, 23);
            this->LoadScreen->TabIndex = 1;
            this->LoadScreen->Text = L"Load Screenshot";
            this->LoadScreen->UseVisualStyleBackColor = true;
            this->LoadScreen->Click += gcnew System::EventHandler(this, &ToSForm::LoadScreen_Click);
            // 
            // calcPathBtn
            // 
            this->calcPathBtn->Location = System::Drawing::Point(96, 309);
            this->calcPathBtn->Name = L"calcPathBtn";
            this->calcPathBtn->Size = System::Drawing::Size(102, 23);
            this->calcPathBtn->TabIndex = 2;
            this->calcPathBtn->Text = L"Calculate Path";
            this->calcPathBtn->UseVisualStyleBackColor = true;
            this->calcPathBtn->Click += gcnew System::EventHandler(this, &ToSForm::calcPath_Click);
            // 
            // scbox
            // 
            this->scbox->Location = System::Drawing::Point(6, 21);
            this->scbox->Name = L"scbox";
            this->scbox->Size = System::Drawing::Size(240, 200);
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
            this->autorunBtn->Text = L"Start";
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
            // scanComboBox
            // 
            this->scanComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->scanComboBox->FormattingEnabled = true;
            this->scanComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"掃描Hue判定(快速)", L"掃描邊界判定" });
            this->scanComboBox->Location = System::Drawing::Point(194, 73);
            this->scanComboBox->Name = L"scanComboBox";
            this->scanComboBox->Size = System::Drawing::Size(138, 20);
            this->scanComboBox->TabIndex = 9;
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
            // AutoRunChk
            // 
            this->AutoRunChk->AutoSize = true;
            this->AutoRunChk->Location = System::Drawing::Point(15, 256);
            this->AutoRunChk->Name = L"AutoRunChk";
            this->AutoRunChk->Size = System::Drawing::Size(67, 16);
            this->AutoRunChk->TabIndex = 10;
            this->AutoRunChk->Text = L"AutoRun";
            this->AutoRunChk->UseVisualStyleBackColor = true;
            // 
            // screenGroup
            // 
            this->screenGroup->Controls->Add(this->scbox);
            this->screenGroup->Location = System::Drawing::Point(809, 20);
            this->screenGroup->Name = L"screenGroup";
            this->screenGroup->Size = System::Drawing::Size(254, 230);
            this->screenGroup->TabIndex = 11;
            this->screenGroup->TabStop = false;
            this->screenGroup->Text = L"擷取畫面";
            // 
            // RunPathBtn
            // 
            this->RunPathBtn->Location = System::Drawing::Point(96, 340);
            this->RunPathBtn->Name = L"RunPathBtn";
            this->RunPathBtn->Size = System::Drawing::Size(102, 23);
            this->RunPathBtn->TabIndex = 13;
            this->RunPathBtn->Text = L"Run Path";
            this->RunPathBtn->UseVisualStyleBackColor = true;
            this->RunPathBtn->Click += gcnew System::EventHandler(this, &ToSForm::RunPathBtn_Click);
            // 
            // pathList
            // 
            this->pathList->FormattingEnabled = true;
            this->pathList->ItemHeight = 12;
            this->pathList->Location = System::Drawing::Point(15, 21);
            this->pathList->Name = L"pathList";
            this->pathList->Size = System::Drawing::Size(75, 352);
            this->pathList->TabIndex = 14;
            this->pathList->SelectedIndexChanged += gcnew System::EventHandler(this, &ToSForm::pathList_SelectedIndexChanged);
            // 
            // pathGroup
            // 
            this->pathGroup->Controls->Add(this->clearPathBtn);
            this->pathGroup->Controls->Add(this->removePathBtn);
            this->pathGroup->Controls->Add(this->addPathBtn);
            this->pathGroup->Controls->Add(this->pathList);
            this->pathGroup->Controls->Add(this->calcPathBtn);
            this->pathGroup->Controls->Add(this->RunPathBtn);
            this->pathGroup->Location = System::Drawing::Point(599, 20);
            this->pathGroup->Name = L"pathGroup";
            this->pathGroup->Size = System::Drawing::Size(204, 388);
            this->pathGroup->TabIndex = 15;
            this->pathGroup->TabStop = false;
            this->pathGroup->Text = L"Path";
            // 
            // removePathBtn
            // 
            this->removePathBtn->Location = System::Drawing::Point(96, 50);
            this->removePathBtn->Name = L"removePathBtn";
            this->removePathBtn->Size = System::Drawing::Size(102, 23);
            this->removePathBtn->TabIndex = 16;
            this->removePathBtn->Text = L"Remove";
            this->removePathBtn->UseVisualStyleBackColor = true;
            this->removePathBtn->Click += gcnew System::EventHandler(this, &ToSForm::removePathBtn_Click);
            // 
            // addPathBtn
            // 
            this->addPathBtn->Location = System::Drawing::Point(96, 21);
            this->addPathBtn->Name = L"addPathBtn";
            this->addPathBtn->Size = System::Drawing::Size(102, 23);
            this->addPathBtn->TabIndex = 15;
            this->addPathBtn->Text = L"Add";
            this->addPathBtn->UseVisualStyleBackColor = true;
            this->addPathBtn->Click += gcnew System::EventHandler(this, &ToSForm::addPathBtn_Click);
            // 
            // clearPathBtn
            // 
            this->clearPathBtn->Location = System::Drawing::Point(96, 79);
            this->clearPathBtn->Name = L"clearPathBtn";
            this->clearPathBtn->Size = System::Drawing::Size(102, 23);
            this->clearPathBtn->TabIndex = 17;
            this->clearPathBtn->Text = L"Clear";
            this->clearPathBtn->UseVisualStyleBackColor = true;
            this->clearPathBtn->Click += gcnew System::EventHandler(this, &ToSForm::clearPathBtn_Click);
            // 
            // ToSForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1105, 414);
            this->Controls->Add(this->pathGroup);
            this->Controls->Add(this->screenGroup);
            this->Controls->Add(this->AutoRunChk);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->autorunBtn);
            this->Controls->Add(this->curposShow);
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
            this->screenGroup->ResumeLayout(false);
            this->pathGroup->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
#pragma region Controls event
    private:
        System::Void ToSForm_Load(System::Object^  sender, System::EventArgs^  e);
        System::Void Stone_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void LoadScreen_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void calcPath_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
        System::Void autorunBtn_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void fixedCombo_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void fixedCount_ValueChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void priorityStone_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void selectStoneType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void attackAll_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void enlargeCalcTime_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void enlargeSc_ValueChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void RunPathBtn_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void pathList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void addPathBtn_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void removePathBtn_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void clearPathBtn_Click(System::Object^  sender, System::EventArgs^  e);
#pragma endregion
    private:
        array<System::Drawing::Image^>^ stonePics;
        array<System::Drawing::Image^>^ stoneEdgePics;

        System::Void updateBoard();
        System::Void updateFinalBoard();
        System::Void markBoard(int x1, int y1, int x2, int y2, int index, int length);
        System::Boolean checkCanRun();
        System::Void loadBoardFromBitmap(Bitmap^ bm);
        System::Void run();
};

}
