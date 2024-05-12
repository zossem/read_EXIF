#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
	wxImage::AddHandler(new wxJPEGHandler);           // Dodajemy handlery do formatow
	m_progres_bar->SetRange(60);
	m_progres_bar->Hide();
}

void GUIMyFrame1::button_load_click( wxCommandEvent& event )
{
wxFileDialog openFileDialog(this, _("Open JPEG file"), "", "", "JPEG files (*.jpg)|*.jpg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
if (openFileDialog.ShowModal() == wxID_CANCEL)
return;

wxImage image;
{
if (!image.LoadFile(openFileDialog.GetPath()))
{
wxMessageBox(_("Nie uda\u0142o si\u0119 za\u0142adowa\u0107 obrazka"));
this->Destroy();
return;
}
else
{
Img_Org = image.Copy();
Img_Copy = Img_Org.Copy();
}
}
m_isEroded = false;
m_isBlur = false;
m_isAnimation = false;
m_checkBox_animation->SetValue(false);
EnableControls();

ReadExif(openFileDialog.GetPath());

}

void GUIMyFrame1::bitton_blur_click( wxCommandEvent& event )
{
if (!m_isBlur) {
Censore();
m_isBlur = !m_isBlur;
}

//panel_OnSize(wxSizeEvent());

}

void GUIMyFrame1::button_erode_click( wxCommandEvent& event )
{
	if (!m_isEroded) {
		Erode();
		m_isEroded = !m_isEroded;
	}

	//panel_OnSize(wxSizeEvent());

}

void GUIMyFrame1::checko_box_check( wxCommandEvent& event )
{
	m_isAnimation = !m_isAnimation;
	if (m_isAnimation) 
	{
		m_button_blur->Disable();
		m_button_erode->Disable();

		this->SetMinSize(this->GetSize());
		this->SetMaxSize(this->GetSize());

		Animate();
	}
	else 
	{
		this->SetMaxSize(wxSize(-1, -1));
		this->SetMinSize(m_mini_size);

		EnableControls();
	}
}

void GUIMyFrame1::Animate()
{
	m_progres_bar->Show();

	const int width = Img_Copy.GetSize().x;
	const int height = Img_Copy.GetSize().y;	
	const int depth = 1;

	float sigma = 100.f;

	cimg_library::CImg<unsigned char> copy_cImg = wxImageToCImg(Img_Copy);

	float color[3] = { 1.f, 1.f, 1.f };

	for (int i = 0; i < 60; i++)
	{
		m_progres_bar->SetValue(i);

		cimg_library::CImg<float> gaussian_1(width, height, depth, 3);
		gaussian_1.draw_gaussian(	(width / 2.f) + (width * 0.35f) * cos(i * M_PI / 60.f),
									(height / 2.f) - (height * 0.3f) * sin(i * M_PI / 60.f),
									sigma,	color);

		cimg_library::CImg<float> gaussian_2(width, height, depth, 3);
		gaussian_2.draw_gaussian((width / 2.f) + (width * 0.35f) * cos((i + 60.f) * M_PI / 60.f),
								(height / 2.f) - (height * 0.3f) * sin((i + 60.f) * M_PI / 60.f),
								sigma,	color);

		gaussian_1 += gaussian_2;

		cimg_library::CImg<float> temp_cImg = copy_cImg;
		temp_cImg.mul(gaussian_1);

		wxImage frame = CImgTowxImage(temp_cImg);

		m_isAnimationFrames[i] = wxBitmap(frame);
	}

	m_progres_bar->Hide();
}


void GUIMyFrame1::panel_OnPaint( wxPaintEvent& event )
{
	Repaint(); 
}

void GUIMyFrame1::panel_OnSize( wxSizeEvent& event )
{
if (Img_Org.IsOk()) {
int borderSize = 5; // default border size
int newWidth = this->GetClientSize().GetWidth() - m_panel1->GetPosition().x - borderSize;
int newHeight = this->GetClientSize().GetHeight() - m_panel1->GetPosition().y - borderSize;
Img_Copy = Img_Org.Scale(newWidth, newHeight);
}

}

void GUIMyFrame1::panel_update( wxUpdateUIEvent& event )
{
Repaint(); 
}


void GUIMyFrame1::Censore()
{
	auto img = wxImageToCImg(Img_Org);

	int x0 = 450, y0 = 10, dx = 125, dy = 80;

	auto cropped = img.get_crop(x0, y0, x0 + dx, y0 + dy).blur(5);
	img.draw_image(x0, y0, cropped);
	Img_Org = CImgTowxImage(img);
}

void GUIMyFrame1::Erode()
{
	auto img = wxImageToCImg(Img_Org);
	img.erode(5);
	Img_Org = CImgTowxImage(img);
}





cimg_library::CImg<unsigned char> GUIMyFrame1::wxImageToCImg(wxImage image)
{
	cimg_library::CImg<unsigned char> c_image(image.GetWidth(), image.GetHeight(), 1, 3); // initialized CImage (width, high, deep- 3'ed dimetion, how many chanels for one pixel) 

	for (size_t x = 0; x < image.GetWidth(); x++)
	{
		for (size_t y = 0; y < image.GetHeight(); y++)
		{
			size_t z = 0;
			c_image(x, y, z, 0) = image.GetRed(x, y);
			c_image(x, y, z, 1) = image.GetGreen(x, y);
			c_image(x, y, z, 2) = image.GetBlue(x, y);
		}
	}

	return c_image;
}

void GUIMyFrame1::EnableControls()
{
	m_button_erode->Enable();
	m_button_blur->Enable();
	m_checkBox_animation->Enable();
}





wxImage GUIMyFrame1::CImgTowxImage(cimg_library::CImg<unsigned char> image)
{
	wxImage temp_image(image.width(), image.height());

	for (size_t x = 0; x < temp_image.GetWidth(); x++)
	{
		for (size_t y = 0; y < temp_image.GetHeight(); y++)
		{
			size_t z = 0;
			temp_image.SetRGB(x, y, image(x, y, z, 0), image(x, y, z, 1), image(x, y, z, 2));
		}
	}

	return temp_image;
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
		if (m_isAnimation)
		{
			dc.DrawBitmap(m_isAnimationFrames[frame_index], 0, 0);
			tick_delay = (tick_delay + 1) % 8;
			if (tick_delay >= 7) frame_index = (frame_index + 1) % 60;
			RefreshRect(wxRect(1, 1, 1, 1), false);

			return;
		}

		wxBitmap bitmap(Img_Copy);
		dc.DrawBitmap(bitmap, 0, 0);
	}
}


void GUIMyFrame1::ReadExif(const char* path)
{
	FREE_IMAGE_FORMAT typ_of_bitmap = FreeImage_GetFileType(path, 0);
	FIBITMAP* loaded_bitmap = FreeImage_Load(typ_of_bitmap, path);

	unsigned int width = FreeImage_GetWidth(loaded_bitmap);
	unsigned int height = FreeImage_GetHeight(loaded_bitmap);

	m_textCtrl1->Clear();
	m_textCtrl1->AppendText(std::string("Rozmiar obrazka: ") + std::to_string(width) + "*" + std::to_string(height) + "\n");

	m_textCtrl1->AppendText("EXIF Info:\n");
	unsigned count;
	if (count = FreeImage_GetMetadataCount(FIMD_EXIF_MAIN, loaded_bitmap)) 
	{
		FITAG* tag = NULL;
		FIMETADATA* pice_of_mata_data = NULL;

		pice_of_mata_data = FreeImage_FindFirstMetadata(FIMD_EXIF_MAIN, loaded_bitmap, &tag);
		bool isNotEnd;
		if (pice_of_mata_data)
			isNotEnd = true;
		else
			isNotEnd = false;

		while (isNotEnd)
		{
			const char* tagKey = FreeImage_GetTagKey(tag);
			const char* tagValue = FreeImage_TagToString(FIMD_EXIF_MAIN, tag);
			m_textCtrl1->AppendText(std::string(tagKey) + ": " + std::string(tagValue) + "\n");

			isNotEnd=FreeImage_FindNextMetadata(pice_of_mata_data, &tag);
		}
		FreeImage_FindCloseMetadata(pice_of_mata_data);
	}
}

