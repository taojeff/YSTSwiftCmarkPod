Pod::Spec.new do |s|
  s.name             = 'YSTSwiftCmarkPod'
  s.version          = '0.29.1.1'
  s.summary          = 'A CocoaPods wrapper for the cmark-gfm library.'
  s.description      = 'This pod provides the cmark-gfm library, including both the core parser and GFM extensions, for use in iOS, macOS, and other Apple platforms.'
  s.homepage         = 'https://github.com/taojeff/YSTSwiftCmarkPod'
  s.license          = { :type => 'BSD-2-Clause', :file => 'COPYING' }
  s.author           = { 'taojeff' => 'https://github.com/taojeff' }
  s.source           = { :git => 'https://github.com/taojeff/YSTSwiftCmarkPod.git', :tag => s.version.to_s }

  s.ios.deployment_target = '12.0'

  # Keep the old Swift import unchanged: import swift_cmark_pod
  s.module_name = 'swift_cmark_pod'

  s.default_subspec = 'cmark_gfm', 'cmark_gfm_extensions'

  s.subspec 'cmark_gfm' do |ss|
    ss.source_files = 'src/**/*.{h,c}'
    ss.preserve_paths = 'src/**/*'
    ss.public_header_files = 'src/include/*.h'
    ss.pod_target_xcconfig = {
      'MODULEMAP_FILE' => '$(PODS_TARGET_SRCROOT)/src/include/module.modulemap'
    }
  end

  s.subspec 'cmark_gfm_extensions' do |ss|
    ss.dependency 'YSTSwiftCmarkPod/cmark_gfm'
    ss.source_files = 'extensions/**/*.{h,c}'
    ss.preserve_paths = 'extensions/**/*'
    ss.public_header_files = 'extensions/include/*.h'
    ss.pod_target_xcconfig = {
      'DEFINES_MODULE' => 'YES',
      'CLANG_ENABLE_MODULES' => 'YES'
    }
  end
end