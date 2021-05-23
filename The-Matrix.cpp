#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

class TheMatrix : public olc::PixelGameEngine
{
public:
	TheMatrix() = default;

private:
	struct Stream
	{
		int nColumn = 0;
		float fPositionY = 0;
		float fSpeed = 0;

		std::string strValue;
	};

	int m_nStreamCount = 70;
	int m_nRowCount = 35;
	int m_nColumnCount = 70;

	std::list<Stream> m_listStreams;

	void PrepareStream(Stream* stream)
	{
		stream->fPositionY = 0;
		stream->nColumn = rand() % (m_nColumnCount + 1);
		stream->fSpeed = rand() % 200 + 80;
		stream->strValue.clear();

		int nStreamLenght = rand() % 50 + 20;
		for ( int i = 0; i < nStreamLenght; i++ )
		{
			stream->strValue.append(1, GetRandomCharacter());
		}
	}

	char GetRandomCharacter()
	{
		return (char)((rand() % 93) + 33);
	}

protected:
	virtual bool OnUserCreate() override
	{
		for ( int i = 0; i < m_nStreamCount; i++ )
		{
			Stream stream;
			PrepareStream(&stream);
			m_listStreams.push_back(stream);
		}

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);

		int nStreamToChangeCharacter = rand() % m_nStreamCount;
		int nCurrentStream = 0;

		int nRowHeight = ScreenHeight() / m_nRowCount;
		int nColumnWidth = ScreenWidth() / m_nColumnCount;

		for ( Stream& stream : m_listStreams )
		{
			stream.fPositionY += stream.fSpeed * fElapsedTime;

			int nHeadCharY = stream.fPositionY / nRowHeight;
			int nStreamX = stream.nColumn * nColumnWidth;
			int nStrLength = stream.strValue.size();

			if ( nCurrentStream == nStreamToChangeCharacter )
			{
				stream.strValue[rand() % nStrLength] = GetRandomCharacter();
			}

			for ( int i = 0; i < nStrLength; i++ )
			{
				int nCurrentCharY = (nHeadCharY - i) * nRowHeight;
				int nCharacterIndex = (i - nHeadCharY) % stream.strValue.size();

				if ( nCurrentCharY >= 0 && nCurrentCharY < ScreenHeight() )
				{
					SetPixelMode(olc::Pixel::Mode::ALPHA);
					olc::Pixel colour;
					if ( i == 0 )
						colour = olc::WHITE;
					else if ( i < 4 )
						colour = olc::Pixel(200, 255, 200);
					else
						colour = olc::Pixel(50, 200, 50);

					colour.a = (255 / nStrLength) * (nStrLength - i);
					DrawString(nStreamX, nCurrentCharY, stream.strValue.substr(nCharacterIndex, 1), colour);
				}
			}

			if ( nHeadCharY - nStrLength > m_nRowCount )
				PrepareStream(&stream);

			nCurrentStream++;
		}

		return true;
	}
};


int main()
{
	TheMatrix matrix;
	matrix.Construct(640, 360, 2, 2);
	matrix.Start();
	
	return 0;
}