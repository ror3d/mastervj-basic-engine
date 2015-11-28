#include "graphics.h"

#include <iostream>
#include <fstream>
#include <comdef.h>

void dxCheckError(HRESULT hr)
{
	_com_error err(hr);
	std::ofstream f("log.txt");
	if (FAILED(hr)) {
		fprintf(stdout, "Error: %s\n",
			err.ErrorMessage());
		//DXGetErrorString(hr), DXGetErrorDescription(hr));
		f << "Error: " << err.ErrorMessage() << "\n";

		throw new std::runtime_error(err.ErrorMessage());
	}
	else
	{
		fprintf(stdout, "No Error\n");
		f << "No Error\n";
	}
}