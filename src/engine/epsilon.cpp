#include <engine/epsilon.hpp>

Epsilon::Epsilon(size_t width, size_t height, size_t samples,
                 cl::Platform platform, cl::Device device,
                 std::string source, std::string output)
{
    /* This will remain constant. */
    this->params.platform = platform;
    this->params.device   = device;
    this->params.source   = source;
    this->params.output   = output;
    this->params.width    = width;
    this->params.height   = height;
    this->params.samples  = samples;

    std::vector<cl::Device> devices;
    devices.push_back(device);

    cl_int error;
    this->params.context = cl::Context(devices,
                                       nullptr, nullptr, nullptr, &error);
    Error::Check(Error::Context, error);

    this->params.queue = cl::CommandQueue(this->params.context, device,
                                          0, &error);
    Error::Check(Error::Queue, error);

    std::string src = "#include <cl/epsilon.cl>";
    cl::Program::Sources sourceCode = cl::Program::Sources(1, std::make_pair(
                                      src.c_str(), src.length() + 1));

    this->params.program = cl::Program(this->params.context, sourceCode,
                                       &error);
    Error::Check(Error::Program, error);

    error = this->params.program.build(devices, "-I cl/");
    // write compilation log here
    Error::Check(Error::Build, error);

    this->params.kernel = cl::Kernel(this->params.program, "clmain", &error);
    Error::Check(Error::Kernel, error);
}
