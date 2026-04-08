#include "MainFrame.h"
#include <wx/stdpaths.h>
#include <wx/config.h>
#include <wx/dir.h>

// コンストラクタ
MainFrame::MainFrame(wxWindow* pParent):
    wxFrame(pParent, wxID_ANY, wxT("Commercial")),
    m_pMediaControl(nullptr)
{
    // 設定
    wxConfig* pConfig =new wxConfig(wxTheApp->GetAppName());
    if( pConfig)
    {
        pConfig->Read(wxT("CurrentVideoFileName"), &m_CurrentVideoFileName, wxEmptyString);
        delete pConfig;
    }

    // メディアコントロールを生成する
    m_pMediaControl = new wxMediaCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    {
        wxSizer* pSizer = new wxBoxSizer(wxVERTICAL);
        pSizer->Add(m_pMediaControl, 1, wxEXPAND|wxALL, 0);
        this->SetSizer(pSizer);
    }
    m_pMediaControl->SetVolume(0);

    m_pMediaControl->Bind(wxEVT_MEDIA_LOADED, &MainFrame::OnLoaded, this);
    m_pMediaControl->Bind(wxEVT_MEDIA_FINISHED, &MainFrame::OnFinished, this);

    this->Bind(wxEVT_SHOW, &MainFrame::OnShow, this);
    m_pMediaControl->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnLeftDown, this);
    m_pMediaControl->Bind(wxEVT_MOTION, &MainFrame::OnMouseMove, this);
    m_pMediaControl->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this);

    // マウスカーソルを非表示にする
    this->SetCursor(wxCURSOR_BLANK);
}

// デストラクタ
MainFrame::~MainFrame()
{
    // 設定
    wxConfig* pConfig =new wxConfig(wxTheApp->GetAppName());
    if( pConfig)
    {
        pConfig->Write(wxT("CurrentVideoFileName"), m_CurrentVideoFileName);
        delete pConfig;
    }




    // メディアコントロール
    if( m_pMediaControl )
    {
        m_pMediaControl->Stop();
    }
}

// 動画ファイル一覧を更新する
void MainFrame::ScanVideoFiles()
{
    // 一覧を消去する
    m_VideoFiles.clear();

    // ホームディレクトリ(/home/pi)を取得する
    wxString homeDir = wxStandardPaths::Get().GetUserConfigDir();

    // コマーシャルフォルダを取得する
    wxString cmDir = wxString::Format(wxT("%s/commercials"), homeDir);

    // コマーシャルフォルダが存在しない場合は作成する
    if( !wxDirExists(cmDir))
    {
        wxDir::Make(cmDir);
    }

    // 一覧を取得する
    wxArrayString cmFiles;
    std::vector<wxString> extensions =
    {
        wxT("*.mp4"),
        wxT("*.mov"),
    };
    for(wxString ext : extensions)
    {
        cmFiles.clear();
        if( 0 < wxDir::GetAllFiles(cmDir, &cmFiles, ext, wxDIR_FILES))
        {
            for(size_t i=0 ; i<cmFiles.size() ; i++)
            {
                m_VideoFiles.push_back(cmFiles[i]);
            }
        }
    }

    // ファイル名でソートする
    std::sort(m_VideoFiles.begin(), m_VideoFiles.end());
}

// 次に再生する動画ファイル名を取得する
wxString MainFrame::GetNextVideoFileName()
{
    // 戻り値の初期化
    wxString ret = wxEmptyString;

    // 動画ファイル一覧が空の場合
    if( m_VideoFiles.size() == 0)
    {
        //
    }
    else
    {
        // 現在再生中のファイル名が空の場合
        if( m_CurrentVideoFileName.IsEmpty())
        {
            ret = m_VideoFiles[0];
        }
        else
        {
            bool found = false;
            for(size_t i=0 ; i<m_VideoFiles.size() ; i++)
            {
                if( m_VideoFiles[i] == m_CurrentVideoFileName)
                {
                    found = true;

                    // 次のファイル
                    size_t next_index = (i + 1) % m_VideoFiles.size();
                    ret = m_VideoFiles[next_index];

                    break;
                }
            }

            if( !found)
            {
                ret = m_VideoFiles[0];
            }
        }
    }

    if( !ret.IsEmpty())
    {
        m_CurrentVideoFileName = ret;
    }

    return ret;
}

// OnLoaded
void MainFrame::OnLoaded(wxMediaEvent& e)
{
    if( m_pMediaControl)
    {
        m_pMediaControl->Play();
    }
}

// OnFinished
void MainFrame::OnFinished(wxMediaEvent& e)
{
    if( m_pMediaControl)
    {
        ScanVideoFiles();
        wxString nextVideoFileName = GetNextVideoFileName();
        if( !nextVideoFileName.IsEmpty())
        {
            m_CurrentVideoFileName = nextVideoFileName;
            m_pMediaControl->Load(nextVideoFileName);
        }
    }
}

// OnShow
void MainFrame::OnShow(wxShowEvent& e)
{
    if( e.IsShown())
    {
        // フルスクリーン表示にする
        this->ShowFullScreen(true);

        ScanVideoFiles();
        wxString nextVideoFileName = GetNextVideoFileName();
        if( !nextVideoFileName.IsEmpty())
        {
            if( m_pMediaControl )
            {
                m_CurrentVideoFileName = nextVideoFileName;
                m_pMediaControl->Load(nextVideoFileName);
            }
        }
    }
    else
    {

    }
}

// OnLeftDown
void MainFrame::OnLeftDown(wxMouseEvent& e)
{
    this->Close();
}

// OnMouseMove
void MainFrame::OnMouseMove(wxMouseEvent& e)
{
    // マウスカーソルを表示する
    this->SetCursor(wxCURSOR_ARROW);
}

// OnKeytDown
void MainFrame::OnKeyDown(wxKeyEvent& e)
{
    this->Close();
}
