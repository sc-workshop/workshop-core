#include "cl.h"

namespace wk::parallel
{
	OpenCL::OpenCL()
	{
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        cl::Platform target;
        for (auto& possible_platform : platforms) {
            std::string version_info = possible_platform.getInfo<CL_PLATFORM_VERSION>();

            if (version_info.find("OpenCL 3.") != std::string::npos)
            {
                target = possible_platform;
                m_version = 3;
                break;
            }
            else if (version_info.find("OpenCL 2.") != std::string::npos)
            {
                target = possible_platform;
				m_version = 2;
            }
        }

        if (target() == 0) {
            // No OpenCL 2.0 or newer platform found
            return;
        }

        m_platform = cl::Platform::setDefault(target);
		m_device = cl::Device::getDefault();
		m_context = cl::Context(m_device);
		cl::Context::setDefault(m_context);

        m_available = m_device.getInfo<CL_DEVICE_AVAILABLE>();
	}

    cl::CommandQueue OpenCL::create_queque() const
    {
        return cl::CommandQueue{ context(), device() };
    }

    OpenCL::operator cl::Context& ()
    {
        return m_context;
    }
}