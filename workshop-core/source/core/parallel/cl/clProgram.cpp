#include "clProgram.h"

namespace wk::parallel
{
	clProgramBuilder::clProgramBuilder(cl::string source)
	{
		OpenCL& cl = OpenCL::Instance();
		if (!cl.is_available()) return;

		m_program = cl::Program(cl.context(), source);
	}

	bool clProgramBuilder::is_valid_program()
	{
		return compile();
	}

	clProgramBuilder::operator cl::Program& ()
	{
		compile();
		return m_program;
	}

	bool clProgramBuilder::compile()
	{
		OpenCL& cl = OpenCL::Instance();
		if (!m_dirty) return true;

		bool success = false;
		try
		{
			cl_int result = m_program.build(cl.device(), compile_flags());
			m_dirty = false;
			success = result == CL_SUCCESS;
		}
		catch (cl::Error& e)
		{
			m_program_log << "OpenCL program build finished with error code " << e.err();
		}

		return success;
	}

	const std::string clProgramBuilder::compile_flags() const
	{
		OpenCL& cl = OpenCL::Instance();
		if (!m_compile_flags.empty()) return m_compile_flags;

		std::stringstream flags;
		{
			flags << "-cl-std=";
			switch (cl.m_version)
			{
			case 3:
				flags << "CL3.0";
				break;
			case 2:
				flags << "CL2.0";
				break;
			default:
				flags << "CL1.2";
				break;
			}
		}

		m_compile_flags = flags.str();
		return m_compile_flags;
	}
}