#include "ToSForm.h"
#include "ToS.h"
#include <ctime>
#include <Windows.h>
#pragma comment(lib, "user32.lib")
//using std::vector;

ToS tos;
POINT base;


System::Void ToSF::ToSForm::ToSForm_Load(System::Object^  sender, System::EventArgs^  e) {
	char newBoard[HEIGHT][WIDTH] = {
		{ DARK, DARK, DARK, HEART, HEART, DARK },
		{ HEART, HEART, DARK, HEART, HEART, DARK },
		{ DARK, DARK, DARK, DARK, DARK, DARK },
		{ DARK, HEART, HEART, DARK, HEART, HEART },
		{ DARK, HEART, HEART, DARK, DARK, DARK }
	};
	tos.setBoard(newBoard);
	// my board
	this->board = gcnew array<System::Windows::Forms::PictureBox^>(30);
	int baseX = 10, baseY = 10;
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 6; ++j) {
			this->board[i * 6 + j] = gcnew System::Windows::Forms::PictureBox();
			this->board[i * 6 + j]->Name = L"stone" + (i * 8 + j).ToString();
			this->board[i * 6 + j]->Size = System::Drawing::Size(40, 40);
			this->board[i * 6 + j]->Location = System::Drawing::Point(baseX + 40 * j, baseY + 40 * i);
			this->board[i * 6 + j]->SizeMode = PictureBoxSizeMode::Zoom;
			this->board[i * 6 + j]->Visible = true;
			this->board[i * 6 + j]->Parent = this;
			this->board[i * 6 + j]->Click += gcnew System::EventHandler(this, &ToSForm::Stone_Click);
		}
	}
	this->updateBoard();
	this->timer1->Interval = 500;
	this->timer1->Start();
	base.x = 1570;
	base.y = 700;
}

System::Void ToSF::ToSForm::outpath_Click(System::Object^  sender, System::EventArgs^  e) {

	vector<std::pair<int, int>> path = tos.findPath();
	vector<COMBO> combos = tos.getCombos();
	this->updateBoard();
	this->comboLab->Text = "Combo:" + combos.size().ToString();
	String^ s = "";
	for (int i = 0; i < path.size(); ++i) {
		s = s+"(" + path[i].first.ToString() + "," + path[i].second.ToString()+") ";
	}

}

System::Void ToSF::ToSForm::Stone_Click(System::Object^  sender, System::EventArgs^  e) {
	PictureBox^ pb = safe_cast<PictureBox^>(sender);
	int idx = Array::IndexOf(this->board, pb);
	int x = idx / 6, y = idx % 6;
	tos.setStone(x, y, (tos.getStone(x, y)) % 6 + 1);
	this->updateBoard();
}

System::Void ToSF::ToSForm::updateBoard() {
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 6; ++j) {
			char sType = tos.getStone(i,j);
			switch (sType)
			{
				case NONE:
					this->board[i * 6 + j]->Image = Image::FromFile("pic/b.png");
					break;
				case FIRE:
					this->board[i * 6 + j]->Image = Image::FromFile("pic/f.png");
					break;
				case WATER:
					this->board[i * 6 + j]->Image = Image::FromFile("pic/w.png");
					break;
				case WOOD:
					this->board[i * 6 + j]->Image = Image::FromFile("pic/p.png");
					break;
				case LIGHT:
					this->board[i * 6 + j]->Image = Image::FromFile("pic/l.png");
					break;
				case DARK:
					this->board[i * 6 + j]->Image = Image::FromFile("pic/d.png");
					break;
				case HEART:
					this->board[i * 6 + j]->Image = Image::FromFile("pic/h.png");
					break;
				default:
					break;
			}
		}
	}
}


System::Void ToSF::ToSForm::LoadScreen_Click(System::Object^  sender, System::EventArgs^  e) {

	
}
System::Void ToSF::ToSForm::timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	POINT p;
	if (GetCursorPos(&p))
	{
		this->curposShow->Text = p.x.ToString() + "," + p.y.ToString();
	}
}

System::Void ToSF::ToSForm::autorunBtn_Click(System::Object^  sender, System::EventArgs^  e) {
	Sleep(50);
	keybd_event(VK_SNAPSHOT, MapVirtualKey(VK_LCONTROL, 0), 0, 0);
	Sleep(50);
	keybd_event(VK_SNAPSHOT, MapVirtualKey(VK_LCONTROL, 0), KEYEVENTF_KEYUP, 0);
	Sleep(300);
	Application::DoEvents();
	Image^ img = Clipboard::GetImage();
	Bitmap^ oribm = (Bitmap^)(img);
	Bitmap^ bm = gcnew Bitmap(380, 320);
	Graphics^ g = Graphics::FromImage(bm);
	System::Drawing::Rectangle srcRect = System::Drawing::Rectangle(base.x - 35, base.y - 35, 380, 320);
	g->DrawImage(oribm, 0, 0, srcRect, GraphicsUnit::Pixel);

	vector<float> hues(6);
	this->dealPic(bm);
	this->scbox->Image = Image::FromHbitmap(bm->GetHbitmap());
	this->scbox->SizeMode = PictureBoxSizeMode::Zoom;
	Application::DoEvents();
	//define colors
	hues[LIGHT - 1] = 30;
	hues[DARK - 1] = 290;
	hues[WATER - 1] = 200;
	hues[FIRE - 1] = 15;
	hues[WOOD - 1] = 100;
	hues[HEART - 1] = 350;
	//
	vector<vector<char>> newBoard(5);
	int WIDTH_DIVEDE = bm->Width / 6, HEIGHT_DIVEDE = bm->Height / 5;

	vector<float> samplingHue(30);
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 6; ++j) {
			unsigned long long tmpR = 0, tmpG = 0, tmpB = 0;
			for (int k = 0; k < HEIGHT_DIVEDE; k += HEIGHT_DIVEDE / 10) {
				for (int h = 0; h < WIDTH_DIVEDE; h += WIDTH_DIVEDE / 10) {
					Color thisColor = bm->GetPixel(j*WIDTH_DIVEDE + h, i*HEIGHT_DIVEDE + k);
					tmpR += thisColor.R;
					tmpG += thisColor.G;
					tmpB += thisColor.B;
				}
			}
			Color tmp;
			tmp = Color::FromArgb(tmpR / 100, tmpG / 100, tmpB / 100);
			samplingHue[i * 6 + j] = tmp.GetHue();
		}
	}


	for (int i = 0; i < 5; ++i) {
		newBoard[i].resize(6);
		for (int j = 0; j < 6; ++j) {
			int minVal = 0x7fffffff, idx = 0;
			for (int k = 0; k < 6; ++k) {
				int tmp = abs(samplingHue[i * 6 + j] - hues[k]);
				if (tmp < minVal) {
					minVal = tmp;
					idx = k;
				}
			}
			newBoard[i][j] = idx + 1;
		}
	}
	tos.setBoard(newBoard);
	this->updateBoard();
	Application::DoEvents();
	srand(time(0));
	vector<std::pair<int, int>> path = tos.findPath();
	vector<COMBO> combos = tos.getCombos();
	this->updateBoard();
	this->comboLab->Text = "Combo:" + combos.size().ToString() + " " + path.size();
	Application::DoEvents();
	SetCursorPos(base.x + path[0].second*60, base.y+ path[0].first * 60);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(50);
	for (int i = 1; i < path.size() - 1; ++i) {
		SetCursorPos(base.x + path[i].second * 60, base.y + path[i].first * 60);
		Sleep(50+rand()%20);
	}
	SetCursorPos(base.x + path[path.size() - 1].second * 60, base.y + path[path.size() - 1].first * 60);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}