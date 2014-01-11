#
# spec file for package [spectemplate]
#
# Copyright (c) 2010 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

Name:           libcppzip
Version:
Release:
Summary:        Library for ...

License:        Apache 2.0
URL:            http://github.com/uboot/CppZip
Source0:        %{name}-%{version}.tar.bz2
# BuildArch:      noarch

BuildRequires:  cmake gcc-c++ libz-devel boost-devel doxygen cppunit-devel
# Requires:       

%description
This package contains the cppzip library for...


%package        -n cppzip-devel
Summary:        Development files for cppzip
Group:          Development/Libraries/C and C++
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    -n cppzip-devel
This package contains libraries and header files for
developing applications that use cppzip.

%package        -n cppzip-doc

Summary:        Documentation for cppzip
Group:          Documentation/Other

%description    -n cppzip-doc
This package contains the documentation and tutorials for the cppzip library.


%prep
%setup -q


%build
%{__mkdir} build
cd build
cmake -DCMAKE_INSTALL_PREFIX='%{_prefix}' \
      -DCMAKE_BUILD_TYPE=Release \
%if "%{?_lib}" == "lib64"
      -DLIB_SUFFIX=64 \
%endif
      -DDOC_INSTALL_DIR=%{_docdir} \
      ..

make %{?_smp_mflags}
make %{?_smp_mflags} doc
make test ARGS="-V"


%install
cd build
make install DESTDIR=%{buildroot}
find %{buildroot} -name '*.la' -exec rm -f {} ';'


%clean
rm -rf %{buildroot}


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(0644, root, root, 0755)
%doc
%{_libdir}/*.so.*

%files -n cppzip-devel
%defattr(0644, root, root, 0755)
%doc
%{_includedir}/*
%{_libdir}/*.so

%files -n cppzip-doc
%defattr(-, root, root, 0755)
%{_docdir}/cppzip

%changelog
