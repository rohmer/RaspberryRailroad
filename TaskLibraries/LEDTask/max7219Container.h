#pragma once

class max7219Container
{
	private:
		char data[80];

	public:
		max7219Container();
		void Draw(int row, int col, bool powerMode);
		void Clear();
		char* GetData() {
			return data;
		};
};