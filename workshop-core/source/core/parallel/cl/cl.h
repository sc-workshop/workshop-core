#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 300
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/opencl.hpp>

#include <vector>
#include <sstream>

namespace wk::parallel
{
	class OpenCL
	{
	public:
		static OpenCL& Instance()
		{
			static OpenCL instance;
			return instance;
		}

	private:
		friend class clProgramBuilder;
		OpenCL();

	public:
		bool is_available() const { return m_available; }
		const cl::Device& device() const { return m_device; }
		const cl::Platform& platform() const { return m_platform; }
		const cl::Context& context() const { return m_context; }

	public:
		cl::CommandQueue create_queue() const;

		operator cl::Context&();

	private:
		cl::Platform m_platform;
		cl::Device m_device;
		cl::Context m_context;

		bool m_available = false;
		int m_version = 2;
	};
}