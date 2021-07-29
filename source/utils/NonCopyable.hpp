#ifndef LITTLEVULKANENGINE_NONCOPYABLE_HPP
#define LITTLEVULKANENGINE_NONCOPYABLE_HPP

namespace lve {

    class NonCopyable {
    protected:
        NonCopyable() = default;

        virtual ~NonCopyable() = default;

    public:
        NonCopyable(const NonCopyable &) = delete;

        NonCopyable(NonCopyable &&) noexcept = default;

        NonCopyable &operator=(const NonCopyable &) = delete;

        NonCopyable &operator=(NonCopyable &&) noexcept = default;
    };

} // namespace lve

#endif //LITTLEVULKANENGINE_NONCOPYABLE_HPP
