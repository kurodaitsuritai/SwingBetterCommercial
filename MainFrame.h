#pragma once

#include <wx/wx.h>
#include <wx/mediactrl.h>

// メインフレーム
class MainFrame : public wxFrame
{
    public:
        MainFrame(wxWindow* pParent);
        virtual ~MainFrame();

    private:
        wxMediaCtrl* m_pMediaControl; // メディアコントロール

        wxString m_CurrentVideoFileName; // 現在再生している動画ファイル名
        std::vector<wxString> m_VideoFiles; // 動画ファイル一覧
        void ScanVideoFiles(); // 動画ファイル一覧を更新する
        wxString GetNextVideoFileName(); // 次に再生する動画ファイル名を取得する

        void OnLoaded(wxMediaEvent& e); // OnLoaded
        void OnFinished(wxMediaEvent& e); // OnFinished

        void OnShow(wxShowEvent& e); // OnShow
        void OnLeftDown(wxMouseEvent& e); // OnLeftDown
        void OnMouseMove(wxMouseEvent& e); // OnMouseMove
        void OnKeyDown(wxKeyEvent& e); // OnKeytDown
};

