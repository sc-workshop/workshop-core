#pragma once

#include "cl.h"

namespace wk::parallel
{
	class clProgramBuilder
	{
	public:
		clProgramBuilder(cl::string source);

	public:
		bool is_valid_program();
		std::string program_log() const { return m_program_log.str(); }
		operator cl::Program& ();

	public:
		bool compile();
		const std::string compile_flags() const;

	private:
		cl::Program m_program;
		bool m_dirty = true;
		std::stringstream m_program_log;
		mutable std::string m_compile_flags;
	};
}