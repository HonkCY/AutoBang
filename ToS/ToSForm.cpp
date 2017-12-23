#pragma once
#include "ToSForm.h"
#include "ToS.h"
#include <ctime>
#include <Windows.h>
#include <atlimage.h>
#include <string>
#pragma comment(lib, "user32.lib")
//using std::vector;
void GetWindowPic(HWND hwnd) {
    //目標設定
    HDC hdc = GetDC(NULL);
    int nBitperpixel = GetDeviceCaps(hdc, BITSPIXEL);

    //邊界限制
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int nWidth = rect.right - rect.left;
    int nHeight = rect.bottom - rect.top;

    //影像截取
    CImage image;
    image.Create(nWidth, nHeight, nBitperpixel);
    BitBlt(image.GetDC(), 0, 0, nWidth, nHeight, hdc, rect.left, rect.top, SRCCOPY);
    ReleaseDC(NULL, hdc);
    image.ReleaseDC();
    image.Save(TEXT("Screenshot.bmp"), Gdiplus::ImageFormatBMP);
}
ToS tos;
POINT base;
POINT windowSize;
int StoneWidth;
int StoneHeight;
void SendMouseDown(HWND hwnd, int x, int y) {
    LPARAM p = x + (y << 16);
    PostMessage(hwnd, WM_LBUTTONDOWN, 0, p);
}
void SendMouseUp(HWND hwnd, int x, int y) {
    LPARAM p = x + (y << 16);
    PostMessage(hwnd, WM_LBUTTONUP, 0, p);
}
void SendMouseMove(HWND hwnd, int x, int y) {
    LPARAM p = x + (y << 16);
    PostMessage(hwnd, WM_MOUSEMOVE, 0, p);
}

System::Void ToSF::ToSForm::ToSForm_Load(System::Object^  sender, System::EventArgs^  e) {
    srand(time(0));
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
	//this->timer1->Interval = 8000;
	//this->timer1->Start();
	// setup here
	base.x = 7;
	base.y = 355;
    windowSize.x = 395;
    windowSize.y = 771;
    StoneWidth = 63;//(windowSize.x - base.x * 2) / 6;
    StoneHeight = 63;
	//
	ToS::isFixedCombo = false;
	ToS::fixedComboCount = 1;
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
    //checkCanRun();
    Image^ img = Image::FromFile("Screenshot.bmp");
    Bitmap^ oribm = (Bitmap^) (img);
    Bitmap^ bm = gcnew Bitmap(StoneWidth * 1, StoneHeight * 1);
    Graphics^ g = Graphics::FromImage(bm);
    g->PixelOffsetMode = Drawing::Drawing2D::PixelOffsetMode::HighQuality;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 6; x++) {
            System::Drawing::Rectangle srcRect = System::Drawing::Rectangle(base.x + getStonePosX(x), base.y + getStonePosY(y), StoneWidth * 1, StoneHeight * 1);
            g->DrawImage(oribm, 0, 0, srcRect, GraphicsUnit::Pixel);
            this->dealPicEdge(bm);
            String^ str;
            str = "imgs/" + x.ToString() + "-" + y.ToString() + "stone.bmp";
            bm->Save(str);
        }
    }
    delete img; // Release.
    this->scbox->Image = bm;
    this->scbox->SizeMode = PictureBoxSizeMode::Zoom;
}
System::Void ToSF::ToSForm::timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	/*POINT p;
	if (GetCursorPos(&p))
	{
		this->curposShow->Text = p.x.ToString() + "," + p.y.ToString();
	}*/
    if (checkCanRun()) {
        Sleep(300);
        this->curposShow->Text = "Running...";
        timer1->Stop();
        this->run();
        this->curposShow->Text = "Waiting...";
        timer1->Start();
    }
}
int ToSF::ToSForm::getStonePosX(int x) {
    return x < 3 ? StoneWidth * x : StoneWidth * x + 1;
}
int ToSF::ToSForm::getStonePosY(int y) {
    return y < 3 ? StoneWidth * y : StoneHeight * y + 1;
}
bool ToSF::ToSForm::checkCanRun() {
    HWND hwnd = FindWindowA(NULL, "BlueStacks");
    if (!hwnd) { MessageBoxA(NULL, "Cannot catch bluestacks", "Suck", MB_OK); return 0; }
    HWND mouseHwnd = GetWindow(hwnd, GW_CHILD);

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    if (windowRect.right - windowRect.left != windowSize.x || windowRect.bottom - windowRect.top != windowSize.y) {
        // Set size of window.
        SetWindowPos(hwnd, NULL, 0, 0, windowSize.x, windowSize.y, SWP_NOMOVE);
        Sleep(300);
        GetWindowRect(hwnd, &windowRect);
    }
    RECT mouseWindowRect;
    GetWindowRect(mouseHwnd, &mouseWindowRect);
    GetWindowPic(hwnd);
    Image^ img = Image::FromFile("Screenshot.bmp");
    Bitmap^ oribm = (Bitmap^) (img);
    Bitmap^ bm = gcnew Bitmap(StoneWidth * 6, StoneHeight * 5);
    Graphics^ g = Graphics::FromImage(bm);
    System::Drawing::Rectangle srcRect = System::Drawing::Rectangle(base.x, base.y, StoneWidth * 6, StoneHeight * 5);
    g->DrawImage(oribm, 0, -1, srcRect, GraphicsUnit::Pixel);
    delete img; // Release.
    vector<float> hues(6);
    this->dealPic(bm);

    int WIDTH_DIVEDE = bm->Width / 6, HEIGHT_DIVEDE = bm->Height / 5;

    float sampling;
    unsigned long long tmpR = 0, tmpG = 0, tmpB = 0, divideTimes = 0;
    Color tmp;
    // 抓右下角Combo!亮度
    for (int i = 3; i < 5; i++) {
        for (int j = 3; j < 6; j++) {
            for (int k = 0; k < HEIGHT_DIVEDE; k++) {
                for (int h = 0; h < WIDTH_DIVEDE; h++) {
                    Color thisColor = bm->GetPixel(j*WIDTH_DIVEDE + h, i*HEIGHT_DIVEDE + k);
                    tmpR += thisColor.R;
                    tmpG += thisColor.G;
                    tmpB += thisColor.B;
                    divideTimes += 1;
                }
            }
        }
    }
    tmp = Color::FromArgb(tmpR / divideTimes, tmpG / divideTimes, tmpB / divideTimes);
    sampling = tmp.GetBrightness();
    if (sampling < 0.2f || sampling > 4.5f) { return false; }

    // 抓左邊直行3顆亮度
    tmpR = 0; tmpG = 0; tmpB = 0; divideTimes = 0;
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < HEIGHT_DIVEDE; k++) {
                for (int h = 0; h < WIDTH_DIVEDE; h++) {
                    Color thisColor = bm->GetPixel(j*WIDTH_DIVEDE + h, i*HEIGHT_DIVEDE + k);
                    tmpR += thisColor.R;
                    tmpG += thisColor.G;
                    tmpB += thisColor.B;
                    divideTimes += 1;
                }
            }
        }
    }
    tmp = Color::FromArgb(tmpR / divideTimes, tmpG / divideTimes, tmpB / divideTimes);
    sampling = tmp.GetBrightness();
    if (sampling < 0.2f || sampling > 4.55f) { return false; }
    
    //抓格子交界處的色調
    tmpR = 0; tmpG = 0; tmpB = 0; divideTimes = 0;
    for (int i = 251; i <= 254; i++) {
        for (int j = 250; j <= 253; j++) {
            Color thisColor = bm->GetPixel(i, j);
            tmpR += thisColor.R;
            tmpG += thisColor.G;
            tmpB += thisColor.B;
            divideTimes += 1;
        }
    }
    tmp = Color::FromArgb(tmpR / divideTimes, tmpG / divideTimes, tmpB / divideTimes);
    sampling = tmp.GetHue();
    if (sampling < 28 || sampling > 30) { return false; } // 29.%^$!&#&
    
    return true;
}
System::Void ToSF::ToSForm::run() {
    HWND hwnd = FindWindowA(NULL, "BlueStacks");
    if (!hwnd) { MessageBoxA(NULL, "Cannot catch bluestacks", "Suck", MB_OK); return; }
    HWND mouseHwnd = GetWindow(hwnd, GW_CHILD);

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    if (windowRect.right - windowRect.left != windowSize.x || windowRect.bottom - windowRect.top != windowSize.y) {
        // Set size of window.
        SetWindowPos(hwnd, NULL, 0, 0, windowSize.x, windowSize.y, SWP_NOMOVE);
        Sleep(300);
        GetWindowRect(hwnd, &windowRect);
    }
    RECT mouseWindowRect;
    GetWindowRect(mouseHwnd, &mouseWindowRect);
    // 傻子模式
    if (this->idiotCheck->Checked) {
        POINT path[] = {
        { 0 , 4 },
        { 1 , 4 },
        { 2 , 4 },
        { 3 , 4 },
        { 4 , 4 },
        { 5 , 4 }
        };
        int x = (windowRect.left - mouseWindowRect.left) + base.x + StoneWidth / 2 + getStonePosX(path[0].x);
        int y = (windowRect.top - mouseWindowRect.top) + base.y + StoneHeight / 2 + getStonePosY(path[0].y);
        SendMouseDown(mouseHwnd, x, y);
        Sleep(50);
        for (int i = 1; i < 6; ++i) {
            x = (windowRect.left - mouseWindowRect.left) + base.x + StoneWidth / 2 + getStonePosX(path[i].x);
            y = (windowRect.top - mouseWindowRect.top) + base.y + StoneHeight / 2 + getStonePosY(path[i].y);
            SendMouseMove(mouseHwnd, x, y);
            Sleep(50 + rand() % 20);
        }
        Sleep(50);
        SendMouseUp(mouseHwnd, x, y);
        return;
    }
    // PrintScreen.
    /*// Show window.
    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    keybd_event(VK_MENU, MapVirtualKey(VK_MENU, 0), 0, 0);
    keybd_event(VK_SNAPSHOT, MapVirtualKey(VK_SNAPSHOT, 0), 0, 0);
    Sleep(50);
    keybd_event(VK_SNAPSHOT, MapVirtualKey(VK_SNAPSHOT, 0), KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, MapVirtualKey(VK_MENU, 0), KEYEVENTF_KEYUP, 0);
    Sleep(300);
    Image^ img = Clipboard::GetImage();*/

    GetWindowPic(hwnd);
    Image^ img = Image::FromFile("Screenshot.bmp");
    Bitmap^ oribm = (Bitmap^) (img);
    Bitmap^ bm = gcnew Bitmap(StoneWidth * 6, StoneHeight * 5);
    Graphics^ g = Graphics::FromImage(bm);
    System::Drawing::Rectangle srcRect = System::Drawing::Rectangle(base.x, base.y, StoneWidth * 6, StoneHeight * 5);
    g->DrawImage(oribm, 0, 0, srcRect, GraphicsUnit::Pixel);
    delete img; // Release.
    this->scbox->Image = bm;
    this->scbox->SizeMode = PictureBoxSizeMode::Zoom;
    Application::DoEvents();

    vector<vector<char>> newBoard(5);

    // 掃描模式
    switch (this->scanComboBox->SelectedIndex) {
    case 0: // 一般模式掃描Hue
    {
        this->dealPic(bm);

        //define colors
        vector<float> hues(6);
        hues[LIGHT - 1] = 30;
        hues[DARK - 1] = 290;
        hues[WATER - 1] = 200;
        hues[FIRE - 1] = 15;
        hues[WOOD - 1] = 100;
        hues[HEART - 1] = 350;

        int WIDTH_DIVEDE = bm->Width / 6, HEIGHT_DIVEDE = bm->Height / 5;

        vector<float> samplingHue(30);
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 6; ++j) {
                unsigned long long tmpR = 0, tmpG = 0, tmpB = 0, divideTimes = 0;
                for (int k = 0; k < HEIGHT_DIVEDE; k += HEIGHT_DIVEDE / 10) {
                    for (int h = 0; h < WIDTH_DIVEDE; h += WIDTH_DIVEDE / 10) {
                        Color thisColor = bm->GetPixel(getStonePosX(j) + h, getStonePosY(i) + k);
                        tmpR += thisColor.R;
                        tmpG += thisColor.G;
                        tmpB += thisColor.B;
                        divideTimes += 1;
                    }
                }
                Color tmp;
                tmp = Color::FromArgb(tmpR / divideTimes, tmpG / divideTimes, tmpB / divideTimes);
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
    }
        break;
    case 1: // 邊界掃描
    {
        this->dealPicEdge(bm);

        ArrayList ^ stones = gcnew ArrayList();
        stones->Add(gcnew Bitmap(Image::FromFile("pic/compare/f.bmp")));
        stones->Add(gcnew Bitmap(Image::FromFile("pic/compare/w.bmp")));
        stones->Add(gcnew Bitmap(Image::FromFile("pic/compare/p.bmp")));
        stones->Add(gcnew Bitmap(Image::FromFile("pic/compare/l.bmp")));
        stones->Add(gcnew Bitmap(Image::FromFile("pic/compare/d.bmp")));
        stones->Add(gcnew Bitmap(Image::FromFile("pic/compare/h.bmp")));
        double minVal = DBL_MAX, idx = 0;
        for (int i = 0; i < 5; ++i) {
            newBoard[i].resize(6);
            for (int j = 0; j < 6; ++j) {
                int minVal = 0x7fffffff, idx = 0;
                Bitmap^ bmCmp = gcnew Bitmap(StoneWidth, StoneHeight);
                Graphics^ gCmp = Graphics::FromImage(bmCmp);
                System::Drawing::Rectangle cmpRect = System::Drawing::Rectangle(getStonePosX(j), getStonePosY(i), StoneWidth, StoneHeight);
                gCmp->DrawImage(bm, 0, 0, cmpRect, GraphicsUnit::Pixel);
                for (int k = 0; k < 6; ++k) {
                    double tmp = this->compareImage(bmCmp, dynamic_cast<Bitmap ^>(stones[k]));
                    if (tmp < minVal) {
                        minVal = tmp;
                        idx = k;
                    }
                }
                newBoard[i][j] = idx + 1;
                delete bmCmp;
            }
        }
        delete[] stones;
    }
        break;
    }
    // 設定盤面
    tos.setBoard(newBoard);
    this->updateBoard();
    Application::DoEvents();
    vector<std::pair<int, int>> path = tos.findPath();
    vector<COMBO> combos = tos.getCombos();
    this->updateBoard();
    this->comboLab->Text = "Combo:" + combos.size().ToString() + " " + path.size();
    Application::DoEvents();
    int x = (windowRect.left - mouseWindowRect.left) + base.x + StoneWidth / 2 + getStonePosX(path[0].second);
    int y = (windowRect.top - mouseWindowRect.top) + base.y + StoneHeight / 2 + getStonePosY(path[0].first);
    SendMouseDown(mouseHwnd, x, y);
    Sleep(50);
    for (int i = 1; i < path.size(); ++i) {
        x = (windowRect.left - mouseWindowRect.left) + base.x + StoneWidth / 2 + getStonePosX(path[i].second);
        y = (windowRect.top - mouseWindowRect.top) + base.y + StoneHeight / 2 + getStonePosY(path[i].first);
        SendMouseMove(mouseHwnd, x, y);
        Sleep(50);
    }
    SendMouseUp(mouseHwnd, x, y);
}
System::Void ToSF::ToSForm::autorunBtn_Click(System::Object^  sender, System::EventArgs^  e) {
    timer1->Interval = 2000;
    if (timer1->Enabled) {
        this->curposShow->Text = "Stop";
        timer1->Stop();
    } else {
        //timer1->Start();
        timer1_Tick(nullptr, nullptr);
    }
}

System::Void ToSF::ToSForm::fixedCombo_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	ToS::isFixedCombo = this->fixedCombo->Checked;
	if (ToS::isFixedCombo) {
		this->fixedCount->Enabled = true;
	}
	else {
		this->fixedCount->Enabled = false;
	}
}
System::Void ToSF::ToSForm::fixedCount_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	ToS::fixedComboCount = safe_cast<int>(this->fixedCount->Value);
}

System::Void ToSF::ToSForm::priorityStone_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	ToS::isPriorityStone = this->priorityStone->Checked;
	this->selectStoneType->Enabled = ToS::isPriorityStone;
}

System::Void ToSF::ToSForm::selectStoneType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	ToS::priorityStoneType = this->selectStoneType->SelectedIndex;
}

System::Void ToSF::ToSForm::attackAll_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	ToS::isAttackAll = this->attackAll->Checked;
}

System::Void ToSF::ToSForm::enlargeCalcTime_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	ToS::isEnlargeCalcTime = this->enlargeCalcTime->Checked;
	this->enlargeSc->Enabled = ToS::isEnlargeCalcTime;
}

System::Void ToSF::ToSForm::enlargeSc_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	ToS::enlargeScale = safe_cast<int>(this->enlargeSc->Value);
}
