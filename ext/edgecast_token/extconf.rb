require 'mkmf'

pkg_config 'openssl'
create_makefile('edgecast_token/edgecast_token')
