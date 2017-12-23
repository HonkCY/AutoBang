#pragma once
#include "ToSForm.h"
#include "ToS.h"
#include <ctime>
#include <Windows.h>
#include <atlimage.h>
#include <string>
#pragma comment(lib, "user32.lib")

HWND hwnd, mouseHwnd;
RECT windowRect, mouseWindowRect;
ToS tos;
POINT base;
POINT windowSize;
int StoneWidth;
int StoneHeight;
#pragma region Winapi
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
bool IsCursorInWindow() {
    POINT cursorPoint;
    if (GetCursorPos(&cursorPoint)) {
        if (cursorPoint.x <= windowRect.right && cursorPoint.x >= windowRect.left &&
            cursorPoint.y <= windowRect.bottom && cursorPoint.y >= windowRect.top) {
            return true;
        }
    }
    return false;
}
void SendMouseDown(HWND _hwnd, int x, int y) {
    if (IsCursorInWindow()) {
        return;
    }
    LPARAM p = x + (y << 16);
    PostMessage(_hwnd, WM_LBUTTONDOWN, 0, p);
}
void SendMouseUp(HWND _hwnd, int x, int y) {
    if (IsCursorInWindow()) {
        return;
    }
    LPARAM p = x + (y << 16);
    PostMessage(_hwnd, WM_LBUTTONUP, 0, p);
}
void SendMouseMove(HWND _hwnd, int x, int y) {
    if (IsCursorInWindow()) {
        return;
    }
    LPARAM p = x + (y << 16);
    PostMessage(_hwnd, WM_MOUSEMOVE, 0, p);
}
bool GetBSHwnd() {
    //if (!hwnd) {
        hwnd = FindWindowA(NULL, "BlueStacks");
        if (!hwnd) { MessageBoxA(NULL, "Cannot catch bluestacks", "Suck", MB_OK); return false; }
        mouseHwnd = GetWindow(hwnd, GW_CHILD);

        GetWindowRect(hwnd, &windowRect);
        if (windowRect.right - windowRect.left != windowSize.x || windowRect.bottom - windowRect.top != windowSize.y) {
            // Set size of window.
            SetWindowPos(hwnd, NULL, 0, 0, windowSize.x, windowSize.y, SWP_NOMOVE);
            Sleep(300);
            GetWindowRect(hwnd, &windowRect);
        }
        GetWindowRect(mouseHwnd, &mouseWindowRect);
    //}
    return true;
}
#pragma endregion

#pragma region Image deal
//! 二值化
void Threshold(System::Drawing::Bitmap ^ bm, unsigned char t) {
    for (int i = 0; i < bm->Height; i++) {
        for (int j = 0; j < bm->Width; j++) {
            System::Drawing::Color tmpColor = bm->GetPixel(j, i);
            double gray;
            gray = tmpColor.R * 0.299 + tmpColor.G * 0.587 + tmpColor.B * 0.114;
            if (gray > t) { gray = 255; } else { gray = 0; }
            bm->SetPixel(j, i, System::Drawing::Color::FromArgb(gray, gray, gray));
        }
    }
}
//! Filtering
void Filtering(System::Drawing::Bitmap ^ bm, double filter[][5]) {
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
                        xOffset = xOffset > (bm->Width - 1) ? ((bm->Width - 1) * 2 - xOffset) : xOffset;
                        yOffset = yOffset < 0 ? -yOffset : yOffset;
                        yOffset = yOffset > (bm->Height - 1) ? ((bm->Height - 1) * 2 - yOffset) : yOffset;
                    }
                    System::Drawing::Color tmpColor = bm->GetPixel(xOffset, yOffset);
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
            bm->SetPixel(j, i, System::Drawing::Color::FromArgb(dstColor[i][j][0], dstColor[i][j][1], dstColor[i][j][2]));
        }
    }
    delete[] dstColor;
}
//! 邊緣化+高斯模糊
void DealPicEdge(System::Drawing::Bitmap ^ bm) {
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
    Filtering(bm, filter);
    Filtering(bm, filter);
    Threshold(bm, 8);
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
    Filtering(bm, gussian);
}
//! 圖片比對 回傳diff
double CompareImage(System::Drawing::Bitmap ^ bm1, System::Drawing::Bitmap ^ bm2) {
    double diff = 0;
    for (int y = 0; y < bm1->Height; y++) {
        for (int x = 0; x < bm1->Width; x++) {
            System::Drawing::Color c1 = bm1->GetPixel(x, y);
            System::Drawing::Color c2 = bm2->GetPixel(x, y);
            diff += (c1.R - c2.R) * (c1.R - c2.R); // already gray, thus only get red.
        }
    }
    return diff;
}
//! Original deal function by chouber.
void DealPic(System::Drawing::Bitmap ^ bm) {
    // Quant_Uniform
    for (int i = 0; i < bm->Height; i++) {
        for (int j = 0; j < bm->Width; j++) {
            System::Drawing::Color thisColor = bm->GetPixel(j, i);
            bm->SetPixel(j, i, System::Drawing::Color::FromArgb((thisColor.R >> 2) << 2, (thisColor.G >> 2) << 2, (thisColor.B >> 2) << 2));
        }
    }
}
#pragma endregion

//! 傳入Index 回傳盤面上的座標
int GetStonePosX(int x) {
    return x < 3 ? StoneWidth * x : StoneWidth * x + 1;
}
int GetStonePosY(int y) {
    return y < 3 ? StoneWidth * y : StoneHeight * y + 1;
}

void IdiotRun() {
    if (!GetBSHwnd()) { return; }
    POINT path[] = {
        { 0 , 4 },
        { 1 , 4 },
        { 2 , 4 },
        { 3 , 4 },
        { 4 , 4 },
        { 5 , 4 }
    };
    int x = (windowRect.left - mouseWindowRect.left) + base.x + StoneWidth / 2 + GetStonePosX(path[0].x);
    int y = (windowRect.top - mouseWindowRect.top) + base.y + StoneHeight / 2 + GetStonePosY(path[0].y);
    SendMouseDown(mouseHwnd, x, y);
    Sleep(50);
    for (int i = 1; i < 6; ++i) {
        x = (windowRect.left - mouseWindowRect.left) + base.x + StoneWidth / 2 + GetStonePosX(path[i].x);
        y = (windowRect.top - mouseWindowRect.top) + base.y + StoneHeight / 2 + GetStonePosY(path[i].y);
        SendMouseMove(mouseHwnd, x, y);
        Sleep(50 + rand() % 20);
    }
    Sleep(50);
    SendMouseUp(mouseHwnd, x, y);
}
// path = <y, x>
void RunPath(vector<std::pair<int, int>> path) {
    int x = (windowRect.left - mouseWindowRect.left) + base.x + StoneWidth / 2 + GetStonePosX(path[0].second);
    int y = (windowRect.top - mouseWindowRect.top) + base.y + StoneHeight / 2 + GetStonePosY(path[0].first);
    SendMouseDown(mouseHwnd, x, y);
    Sleep(50);
    for (int i = 1; i < path.size(); ++i) {
        x = (windowRect.left - mouseWindowRect.left) + base.x + StoneWidth / 2 + GetStonePosX(path[i].second);
        y = (windowRect.top - mouseWindowRect.top) + base.y + StoneHeight / 2 + GetStonePosY(path[i].first);
        SendMouseMove(mouseHwnd, x, y);
        Sleep(50);
    }
    SendMouseUp(mouseHwnd, x, y);
}


System::Void ToSF::ToSForm::ToSForm_Load(System::Object^  sender, System::EventArgs^  e) {
    srand(time(0));

    // Init
    this->selectStoneType->SelectedIndex = 0;
    this->scanComboBox->SelectedIndex = 0;

    this->stonePics = gcnew array<System::Drawing::Image^>(6);
    this->stonePics[0] = Image::FromFile("pic/f.png");
    this->stonePics[1] = Image::FromFile("pic/w.png");
    this->stonePics[2] = Image::FromFile("pic/p.png");
    this->stonePics[3] = Image::FromFile("pic/l.png");
    this->stonePics[4] = Image::FromFile("pic/d.png");
    this->stonePics[5] = Image::FromFile("pic/h.png");

    this->stoneEdgePics = gcnew array<System::Drawing::Image^>(6);
    this->stoneEdgePics[0] = Image::FromFile("pic/compare/f.bmp");
    this->stoneEdgePics[1] = Image::FromFile("pic/compare/w.bmp");
    this->stoneEdgePics[2] = Image::FromFile("pic/compare/p.bmp");
    this->stoneEdgePics[3] = Image::FromFile("pic/compare/l.bmp");
    this->stoneEdgePics[4] = Image::FromFile("pic/compare/d.bmp");
    this->stoneEdgePics[5] = Image::FromFile("pic/compare/h.bmp");

    char newBoard[HEIGHT][WIDTH] = {
        { DARK, DARK, DARK, HEART, HEART, DARK },
        { HEART, HEART, DARK, HEART, HEART, DARK },
        { DARK, DARK, DARK, DARK, DARK, DARK },
        { DARK, HEART, HEART, DARK, HEART, HEART },
        { DARK, HEART, HEART, DARK, DARK, DARK }
    };
    tos.setSrcBoard(newBoard);
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
    this->finalBoard = gcnew array<System::Windows::Forms::PictureBox^>(30);
    baseX = 300, baseY = 10;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            this->finalBoard[i * 6 + j] = gcnew System::Windows::Forms::PictureBox();
            this->finalBoard[i * 6 + j]->Name = L"stone" + (i * 8 + j).ToString();
            this->finalBoard[i * 6 + j]->Size = System::Drawing::Size(40, 40);
            this->finalBoard[i * 6 + j]->Location = System::Drawing::Point(baseX + 40 * j, baseY + 40 * i);
            this->finalBoard[i * 6 + j]->SizeMode = PictureBoxSizeMode::Zoom;
            this->finalBoard[i * 6 + j]->Visible = true;
            this->finalBoard[i * 6 + j]->Parent = this;
            //this->finalBoard[i * 6 + j]->Click += gcnew System::EventHandler(this, &ToSForm::Stone_Click);
        }
    }
    this->updateFinalBoard();
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

System::Void ToSF::ToSForm::calcPath_Click(System::Object^  sender, System::EventArgs^  e) {
    this->updateBoard();
    vector<std::pair<int, int>> path = tos.findPath();
    vector<COMBO> combos = tos.getCombos();
    //this->updateBoard();
    this->updateFinalBoard();
    this->comboLab->Text = "Combo:" + combos.size().ToString() + ", Path:" + path.size();
    for (int i = 0; i < path.size() - 1; i++) {
        markBoard(path[i].second, path[i].first, path[i + 1].second, path[i + 1].first, i, path.size());
    }
    // Record path list.
    this->pathList->Items->Clear();
    for (int i = 0; i < path.size(); ++i) {
        this->pathList->Items->Add(path[i].first.ToString() + "," + path[i].second.ToString());
    }
}

System::Void ToSF::ToSForm::Stone_Click(System::Object^  sender, System::EventArgs^  e) {
    PictureBox^ pb = safe_cast<PictureBox^>(sender);
    int idx = Array::IndexOf(this->board, pb);
    int y = idx / 6, x = idx % 6;
    if (this->addPathBtn->BackColor == SystemColors::ActiveCaption) { // Add mode.
        this->pathList->Items->Add(y.ToString() + "," + x.ToString());
        return;
    }
    tos.setStone(y, x, (tos.getStone(y, x)) % 6 + 1);
    this->updateBoard();
}

System::Void ToSF::ToSForm::LoadScreen_Click(System::Object^  sender, System::EventArgs^  e) {
    if (!GetBSHwnd()) { return; }
    GetWindowPic(hwnd);
    Image^ img = Image::FromFile("Screenshot.bmp");
    Bitmap^ oribm = (Bitmap^) (img);
    Bitmap^ bm = gcnew Bitmap(StoneWidth * 6, StoneHeight * 5);
    Graphics^ g = Graphics::FromImage(bm);
    g->PixelOffsetMode = Drawing::Drawing2D::PixelOffsetMode::HighQuality;
    System::Drawing::Rectangle srcRect = System::Drawing::Rectangle(base.x, base.y, StoneWidth * 6, StoneHeight * 5);
    g->DrawImage(oribm, 0, 0, srcRect, GraphicsUnit::Pixel);
    delete img; // Release.
    this->scbox->Image = bm;
    this->scbox->SizeMode = PictureBoxSizeMode::Zoom;
    Application::DoEvents();
    this->loadBoardFromBitmap(bm);
}
/*
void testing() {
    //checkCanRun();
    Image^ img = Image::FromFile("Screenshot.bmp");
    Bitmap^ oribm = (Bitmap^) (img);
    Bitmap^ bm = gcnew Bitmap(StoneWidth * 1, StoneHeight * 1);
    Graphics^ g = Graphics::FromImage(bm);
    g->PixelOffsetMode = Drawing::Drawing2D::PixelOffsetMode::HighQuality;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 6; x++) {
            System::Drawing::Rectangle srcRect = System::Drawing::Rectangle(base.x + GetStonePosX(x), base.y + GetStonePosY(y), StoneWidth * 1, StoneHeight * 1);
            g->DrawImage(oribm, 0, 0, srcRect, GraphicsUnit::Pixel);
            DealPicEdge(bm);
            String^ str;
            str = "imgs/" + x.ToString() + "-" + y.ToString() + "stone.bmp";
            bm->Save(str);
        }
    }
    delete img; // Release.
    this->scbox->Image = bm;
    this->scbox->SizeMode = PictureBoxSizeMode::Zoom;
}*/
System::Void ToSF::ToSForm::timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
    if (checkCanRun()) {
        Sleep(300);
        this->curposShow->Text = "Running...";
        timer1->Stop();
        this->run();
        // AutoRunChk .
        if (this->AutoRunChk->Checked) {
            this->curposShow->Text = "Waiting...";
            timer1->Start();
        } else {
            this->curposShow->Text = "";
        }
    } else {
        if (this->AutoRunChk->Checked) {
            if (this->curposShow->Text != "Waiting...") {
                this->curposShow->Text = "Waiting...";
            }
        } else {
            this->autorunBtn->Text = "Start";
            this->curposShow->Text = "Cannot run.";
        }
    }
}

System::Void ToSF::ToSForm::autorunBtn_Click(System::Object^  sender, System::EventArgs^  e) {
    timer1->Interval = 2000;
    if (timer1->Enabled) {
        timer1->Stop();
        this->curposShow->Text = "Stop";
        this->autorunBtn->Text = "Start";
    } else {
        //timer1->Start();
        this->autorunBtn->Text = "Stop";
        timer1_Tick(nullptr, nullptr);
        timer1->Start();
    }
}

System::Void ToSF::ToSForm::fixedCombo_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
    ToS::isFixedCombo = this->fixedCombo->Checked;
    if (ToS::isFixedCombo) {
        this->fixedCount->Enabled = true;
    } else {
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

System::Void ToSF::ToSForm::RunPathBtn_Click(System::Object ^ sender, System::EventArgs ^ e) {
    if (!GetBSHwnd()) { return; }
    vector<std::pair<int, int>> path;
    for (int i = 0; i < this->pathList->Items->Count; i++) {
        int y = Convert::ToInt32(this->pathList->Items[i]->ToString()->Split(',')[0]);
        int x = Convert::ToInt32(this->pathList->Items[i]->ToString()->Split(',')[1]);
        path.push_back({ y,x });
    }
    RunPath(path);
}
System::Void ToSF::ToSForm::pathList_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
    int i = this->pathList->SelectedIndex;
    if (i == -1) { return; }

    int y = Convert::ToInt32(this->pathList->Items[i]->ToString()->Split(',')[0]);
    int x = Convert::ToInt32(this->pathList->Items[i]->ToString()->Split(',')[1]);
    for (int h = 0; h < 5; h++) {
        for (int w = 0; w < 6; w++) {
            this->board[h * 6 + w]->BorderStyle = BorderStyle::None;
        }
    }
    this->board[y * 6 + x]->BorderStyle = BorderStyle::Fixed3D;
}
System::Void ToSF::ToSForm::addPathBtn_Click(System::Object ^ sender, System::EventArgs ^ e) {
    this->addPathBtn->BackColor = this->addPathBtn->BackColor == SystemColors::ActiveCaption ? SystemColors::Control : SystemColors::ActiveCaption;
}
System::Void ToSF::ToSForm::removePathBtn_Click(System::Object ^ sender, System::EventArgs ^ e) {
    int i = this->pathList->SelectedIndex;
    if (i >= 0) {
        this->pathList->Items->RemoveAt(i);
        if (i < this->pathList->Items->Count) {
            this->pathList->SelectedIndex = i;
        }
    }
}
System::Void ToSF::ToSForm::clearPathBtn_Click(System::Object ^ sender, System::EventArgs ^ e) {
    this->pathList->Items->Clear();
}
System::Void ToSF::ToSForm::updateBoard() {
    tos.initBoard(); // 僅顯示初始盤面
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            char sType = tos.getStone(i, j);
            if (sType > 0) {
                this->board[i * 6 + j]->Image = stonePics[sType - 1];
            } else {
                this->board[i * 6 + j]->Image = nullptr;
            }
        }
    }
}
System::Void ToSF::ToSForm::updateFinalBoard() {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            char sType = tos.getStone(i, j);
            if (sType > 0) {
                this->finalBoard[i * 6 + j]->Image = stonePics[sType - 1];
            } else {
                this->finalBoard[i * 6 + j]->Image = nullptr;
            }
        }
    }
}
System::Void ToSF::ToSForm::markBoard(int x1, int y1, int x2, int y2, int index, int length) {
    int movingVector[2] = { x2 - x1, y2 - y1 };
    int imgWidth = this->board[y1 * 6 + x1]->Image->Width;
    int imgHeight = this->board[y1 * 6 + x1]->Image->Height;
    int imgCenterX = imgWidth / 2;
    int imgCenterY = imgHeight / 2;

    int lineBorder = imgWidth / 10;

    Bitmap^ bitmap1 = gcnew Bitmap(this->board[y1 * 6 + x1]->Image);
    Graphics^ g1 = Graphics::FromImage(bitmap1);
    if (index == 0) { // Start point
        int rectSize = imgWidth / 2;
        g1->FillRectangle(Brushes::Black, imgCenterX - rectSize / 2, imgCenterY - rectSize / 2, rectSize, rectSize);
    }
    g1->DrawLine(gcnew Pen(Color::FromArgb(index * (255 / length), index * (255 / length), index * (255 / length)), lineBorder), imgCenterX, imgCenterY, imgCenterX + movingVector[0] * (imgWidth / 2), imgCenterY + movingVector[1] * (imgHeight / 2));

    Bitmap^ bitmap2 = gcnew Bitmap(this->board[y2 * 6 + x2]->Image);
    Graphics^ g2 = Graphics::FromImage(bitmap2);
    g2->DrawLine(gcnew Pen(Color::FromArgb(index * (255 / length), index * (255 / length), index * (255 / length)), lineBorder), imgCenterX - movingVector[0] * (imgWidth / 2), imgCenterY - movingVector[1] * (imgHeight / 2), imgCenterX, imgCenterY);
    if (index == length - 2) { // End point.
        int rectSize = imgWidth / 3;
        g2->FillEllipse(Brushes::White, imgCenterX - rectSize / 2, imgCenterY - rectSize / 2, rectSize, rectSize);
    }
    //g->DrawString(index.ToString(), gcnew System::Drawing::Font("Arial", 36), Brushes::Black, 12 + (index % 10) * 6, 12 + (index / 10) * 6);
    this->board[y1 * 6 + x1]->Image = bitmap1;
    this->board[y2 * 6 + x2]->Image = bitmap2;
}
System::Boolean ToSF::ToSForm::checkCanRun() {
    if (!GetBSHwnd()) { return false; }
    GetWindowPic(hwnd);
    Image^ img = Image::FromFile("Screenshot.bmp");
    Bitmap^ oribm = (Bitmap^) (img);
    Bitmap^ bm = gcnew Bitmap(StoneWidth * 6, StoneHeight * 5);
    Graphics^ g = Graphics::FromImage(bm);
    g->PixelOffsetMode = Drawing::Drawing2D::PixelOffsetMode::HighQuality;
    System::Drawing::Rectangle srcRect = System::Drawing::Rectangle(base.x, base.y, StoneWidth * 6, StoneHeight * 5);
    g->DrawImage(oribm, 0, 0, srcRect, GraphicsUnit::Pixel);
    delete img; // Release.
    vector<float> hues(6);
    DealPic(bm);

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
System::Void ToSF::ToSForm::loadBoardFromBitmap(System::Drawing::Bitmap^ bm) {
    vector<vector<char>> newBoard(5);
    // 掃描模式
    switch (this->scanComboBox->SelectedIndex) {
    case 0: // 一般模式掃描Hue
    {
        DealPic(bm);

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
                        System::Drawing::Color thisColor = bm->GetPixel(GetStonePosX(j) + h, GetStonePosY(i) + k);
                        tmpR += thisColor.R;
                        tmpG += thisColor.G;
                        tmpB += thisColor.B;
                        divideTimes += 1;
                    }
                }
                System::Drawing::Color tmp;
                tmp = System::Drawing::Color::FromArgb(tmpR / divideTimes, tmpG / divideTimes, tmpB / divideTimes);
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
        DealPicEdge(bm);
        // define image
        
        array<Bitmap^>^ stones = gcnew array<Bitmap^>(6);
        for (int i = 0; i < 6; i++) {
            stones[i] = gcnew Bitmap(this->stoneEdgePics[i]);
        }
        double minVal = DBL_MAX, idx = 0;
        for (int i = 0; i < 5; ++i) {
            newBoard[i].resize(6);
            for (int j = 0; j < 6; ++j) {
                int minVal = 0x7fffffff, idx = 0;
                Bitmap^ bmCmp = gcnew Bitmap(StoneWidth, StoneHeight);
                Graphics^ gCmp = Graphics::FromImage(bmCmp);
                System::Drawing::Rectangle cmpRect = System::Drawing::Rectangle(GetStonePosX(j), GetStonePosY(i), StoneWidth, StoneHeight);
                gCmp->DrawImage(bm, 0, 0, cmpRect, GraphicsUnit::Pixel);
                for (int k = 0; k < 6; ++k) {
                    double tmp = CompareImage(bmCmp, stones[k]);
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
    tos.setSrcBoard(newBoard);
    this->updateBoard();
    Application::DoEvents();
}
System::Void ToSF::ToSForm::run() {
    if (!GetBSHwnd()) { return; }
    // 傻子模式
    if (this->idiotCheck->Checked) {
        IdiotRun();
        return;
    }
    GetWindowPic(hwnd);
    Image^ img = Image::FromFile("Screenshot.bmp");
    Bitmap^ oribm = (Bitmap^) (img);
    Bitmap^ bm = gcnew Bitmap(StoneWidth * 6, StoneHeight * 5);
    Graphics^ g = Graphics::FromImage(bm);
    g->PixelOffsetMode = Drawing::Drawing2D::PixelOffsetMode::HighQuality;
    System::Drawing::Rectangle srcRect = System::Drawing::Rectangle(base.x, base.y, StoneWidth * 6, StoneHeight * 5);
    g->DrawImage(oribm, 0, 0, srcRect, GraphicsUnit::Pixel);
    delete img; // Release.
    this->scbox->Image = bm;
    this->scbox->SizeMode = PictureBoxSizeMode::Zoom;
    Application::DoEvents();
    this->loadBoardFromBitmap(bm);

    vector<std::pair<int, int>> path = tos.findPath();
    vector<COMBO> combos = tos.getCombos();
    //this->updateBoard();
    this->updateFinalBoard();

    this->comboLab->Text = "Combo:" + combos.size().ToString() + ", Path:" + path.size();
    for (int i = 0; i < path.size() - 1; i++) {
        markBoard(path[i].second, path[i].first, path[i+1].second, path[i+1].first, i, path.size());
    }
    Application::DoEvents();
    RunPath(path);
}


