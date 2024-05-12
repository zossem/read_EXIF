#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{

}

void GUIMyFrame1::button_load_click( wxCommandEvent& event )
{
wxFileDialog saveFileDialog(this, "Choose a file", "", "", "Image Files (*.jpg)|*.jpg");
if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

auto path = saveFileDialog.GetPath().ToStdString();

ProcessImageInit(path);
}

void GUIMyFrame1::bitton_blur_click( wxCommandEvent& event )
{
if (!_censorship) {
Censore();
_censorship = !_censorship;
}

//panel_OnSize(wxSizeEvent());

event.Skip();
}

void GUIMyFrame1::button_erode_click( wxCommandEvent& event )
{
if (!_erode) {
Erode();
_erode = !_erode;
}

//panel_OnSize(wxSizeEvent());

event.Skip();
}

void GUIMyFrame1::checko_box_check( wxCommandEvent& event )
{
if (_animation = !_animation) {
m_button_blur->Disable();
m_button_erode->Disable();
this->SetMinSize(this->GetSize());
this->SetMaxSize(this->GetSize());

Animate();
}
else {
this->SetMaxSize(wxSize(-1, -1));
this->SetMinSize(_minSize);
EnableControls();
}

event.Skip();
}

void GUIMyFrame1::panel_OnPaint( wxPaintEvent& event )
{
Repaint(); event.Skip();
}

void GUIMyFrame1::panel_OnSize( wxSizeEvent& event )
{
if (Img_Org.IsOk()) {
int borderSize = 5; // default border size
int newWidth = this->GetClientSize().GetWidth() - m_panel1->GetPosition().x - borderSize;
int newHeight = this->GetClientSize().GetHeight() - m_panel1->GetPosition().y - borderSize;
Img_Copy = Img_Org.Scale(newWidth, newHeight);
}

event.Skip();
}

void GUIMyFrame1::panel_update( wxUpdateUIEvent& event )
{
Repaint(); event.Skip();
}


void GUIMyFrame1::ProcessImageInit(wxString path)
{
	auto stdStringPath = path.ToStdString();
	auto charPath = path.data();

	if (!Img_Org.LoadFile(path))
	{
		wxMessageBox(_("Nie udalo sie zaladowac obrazka"));
		this->Destroy();
		return;
	}
	Img_Copy = Img_Org.Copy();

	_erode = _censorship = _animation = false;
	m_checkBox_animation->SetValue(false);

	EnableControls();

	ProcessExif(charPath);

	//panel_OnSize(wxSizeEvent());
}

wxImage GUIMyFrame1::CImgTowxImage(cimg_library::CImg<unsigned char> image)
{
	wxImage img(image.width(), image.height());

	for (size_t x = 0; x < img.GetWidth(); x++)
	{
		for (size_t y = 0; y < img.GetHeight(); y++)
		{
			img.SetRGB(x, y, image(x, y, 0, 0), image(x, y, 0, 1), image(x, y, 0, 2));
		}
	}

	return img;
}

cimg_library::CImg<unsigned char> GUIMyFrame1::wxImageToCImg(wxImage image)
{
	cimg_library::CImg<unsigned char> img(image.GetWidth(), image.GetHeight(), 1, 3);

	for (size_t x = 0; x < image.GetWidth(); x++)
	{
		for (size_t y = 0; y < image.GetHeight(); y++)
		{
			img(x, y, 0, 0) = image.GetRed(x, y);
			img(x, y, 0, 1) = image.GetGreen(x, y);
			img(x, y, 0, 2) = image.GetBlue(x, y);
		}
	}

	return img;
}

void GUIMyFrame1::Repaint()
{
	static unsigned frame_index = 0;
	static unsigned tick_delay = 0;

	wxClientDC dc1(m_panel1);
	wxBufferedDC dc(&dc1);
	dc.Clear();

	if (Img_Copy.IsOk())
	{
		if (_animation)
		{
			dc.DrawBitmap(_animationFrames[frame_index], 0, 0);
			tick_delay = (tick_delay + 1) % 8;
			if (tick_delay >= 7) frame_index = (frame_index + 1) % 60;
			RefreshRect(wxRect(1, 1, 1, 1), false);

			return;
		}

		wxBitmap bitmap(Img_Copy);
		dc.DrawBitmap(bitmap, 0, 0);
	}
}

void GUIMyFrame1::EnableControls()
{
	m_button_erode->Enable();
	m_button_blur->Enable();
	m_checkBox_animation->Enable();
}

void GUIMyFrame1::ProcessExif(const char* path)
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path, 0);
	FIBITMAP* dib = FreeImage_Load(fif, path);

	unsigned int width = FreeImage_GetWidth(dib);
	unsigned int height = FreeImage_GetHeight(dib);

	m_textCtrl1->Clear();
	m_textCtrl1->AppendText(std::string("Image size: ") + std::to_string(width) + "x" + std::to_string(height) + "\n\n");

	m_textCtrl1->AppendText("EXIF Info:\n");
	if (int count = FreeImage_GetMetadataCount(FIMD_EXIF_MAIN, dib))
	{
		DisplayExif(dib, FIMD_EXIF_MAIN);
	}
}

void GUIMyFrame1::DisplayExif(FIBITMAP* dib, FREE_IMAGE_MDMODEL model)
{
	FITAG* tag = NULL;
	FIMETADATA* mdhandle = NULL;

	mdhandle = FreeImage_FindFirstMetadata(model, dib, &tag);

	if (mdhandle) {

		do {
			const char* tagKey = FreeImage_GetTagKey(tag);
			const char* tagValue = FreeImage_TagToString(model, tag);

			std::string exifStr = std::string(tagKey) + ": " + std::string(tagValue) + "\n";

			m_textCtrl1->AppendText(exifStr);

		} while (FreeImage_FindNextMetadata(mdhandle, &tag));
	}

	FreeImage_FindCloseMetadata(mdhandle);
}

void GUIMyFrame1::Animate()
{
	m_progres_bar->Show();

	const int height = Img_Copy.GetSize().y;
	const int width = Img_Copy.GetSize().x;

	auto copy = wxImageToCImg(Img_Copy);

	float color[3] = { 1, 1, 1 };

	for (int i = 0; i < 60; i++)
	{
		m_progres_bar->SetValue(i);

		auto gauss = cimg_library::CImg<float>(width, height, 1, 3);
		auto gauss2 = cimg_library::CImg<float>(width, height, 1, 3);

		gauss.draw_gaussian(
			(width / 2.) + (width * 0.3) * cos(i * M_PI / 60.f),
			(height / 2.) + (height * 0.3) * sin(i * M_PI / 60.f),
			120.0f,
			color,
			1.0f);

		gauss2.draw_gaussian(
			(width / 2.) + (width * 0.3) * cos((i + 60.f) * M_PI / 60.f),
			(height / 2.) + (height * 0.3) * sin((i + 60.f) * M_PI / 60.f),
			120.0f,
			color,
			1.0f);

		gauss += gauss2;

		cimg_library::CImg<float> tmp = copy;
		tmp.mul(gauss);
		auto frame = CImgTowxImage(tmp);

		_animationFrames[59 - i] = wxBitmap(frame);
	}

	m_progres_bar->Hide();
}

void GUIMyFrame1::Erode()
{
	auto img = wxImageToCImg(Img_Org);
	img.erode(5);
	Img_Org = CImgTowxImage(img);
}

void GUIMyFrame1::Censore()
{
	auto img = wxImageToCImg(Img_Org);

	int x0 = 450, y0 = 10, dx = 125, dy = 80;

	auto cropped = img.get_crop(x0, y0, x0 + dx, y0 + dy).blur(5);
	img.draw_image(x0, y0, cropped);
	Img_Org = CImgTowxImage(img);
}