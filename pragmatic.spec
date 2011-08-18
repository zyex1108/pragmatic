Summary:   Anisotropic mesh adaptivity
Name:	   pragmatic
Version:   0.1
Release:   0
License:   LGPL
Group:	   Applications/Programming
Vendor:    Applied Modelling and Computation Group (AMCG), Imperial College London
Packager:  Gerard Gorman <g.gorman@imperial.ac.uk>
URL:	   http://amcg.ese.ic.ac.uk
Source0:   %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Prefix:	   /usr


%description
2D and 3D anisotropic mesh adaptivity.

%prep
rm -rf $RPM_BUILD_DIR/%{name}-%{version}
zcat $RPM_SOURCE_DIR/%{name}-%{version}.tar.gz | tar -xvf -

%setup -q

%build
%configure
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files -f INSTALLED_FILES
/usr/bin/terreno.bin

%defattr(-,root,root,-)

%changelog
* Wed Aug 17 2011 Gerard Gorman <g.gorman@imperial.ac.uk> - 
- Beta release.
