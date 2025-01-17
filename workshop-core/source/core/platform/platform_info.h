#pragma once

namespace wk
{
	enum Platform
	{
		Unknown,
		Windows // support for other platfrom in coming in 2077
	};

	constexpr Platform CurrentPlatform()
	{
#ifdef _WINDOWS
		return Platform::Windows;
#else
		return Platform::Unknown;
#endif
	}

	constexpr const char* CurrentPlatformName()
	{
		switch (CurrentPlatform())
		{
		case Platform::Windows:
			return "Windows";
		default:
			return "Unknown";
		}
	}
}