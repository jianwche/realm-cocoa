# NOTE: THIS SCRIPT IS SUPPOSED TO RUN IN A POSIX COMPATIBLE SHELL

source="$1"
target="$2"

get_config_param()
{
    local name line value
    name="$1"
    if ! line="$(grep "^$name *=" "$source")"; then
        cat 1>&2 <<EOF
ERROR: Failed to read configuration parameter '$name'.
Maybe you need to rerun 'sh build.sh config [PREFIX]'.
EOF
        return 1
    fi
    value="$(printf "%s\n" "$line" | cut -d= -f2-)" || return 1
    value="$(printf "%s\n" "$value" | sed 's/^ *//')" || return 1
    printf "%s\n" "$value"
}

cstring_escape()
{
    local str
    str="$1"
    str="$(printf "%s\n" "$str" | sed "s/\\([\"\\]\\)/\\\\\\1/g")" || return 1
    printf "%s\n" "$str"

}

tightdb_version="$(get_config_param "TIGHTDB_VERSION")" || exit 1
tigthdb_version_escaped="$(cstring_escape "$tightdb_version")" || exit 1

enable_replication="$(get_config_param "ENABLE_REPLICATION")" || exit 1
if [ "$enable_replication" ]; then
    enable_replication="1"
else
    enable_replication="0"
fi

install_prefix="$(get_config_param "INSTALL_PREFIX")" || exit 1
install_prefix_escaped="$(cstring_escape "$install_prefix")" || exit 1

install_exec_prefix="$(get_config_param "INSTALL_EXEC_PREFIX")" || exit 1
install_exec_prefix_escaped="$(cstring_escape "$install_exec_prefix")" || exit 1

install_includedir="$(get_config_param "INSTALL_INCLUDEDIR")" || exit 1
install_includedir_escaped="$(cstring_escape "$install_includedir")" || exit 1

install_bindir="$(get_config_param "INSTALL_BINDIR")" || exit 1
install_bindir_escaped="$(cstring_escape "$install_bindir")" || exit 1

install_libdir="$(get_config_param "INSTALL_LIBDIR")" || exit 1
install_libdir_escaped="$(cstring_escape "$install_libdir")" || exit 1

install_libexecdir="$(get_config_param "INSTALL_LIBEXECDIR")" || exit 1
install_libexecdir_escaped="$(cstring_escape "$install_libexecdir")" || exit 1

cat >"$target" <<EOF
/*************************************************************************
 *
 * CAUTION:  DO NOT EDIT THIS FILE -- YOUR CHANGES WILL BE LOST!
 *
 * This file is generated by config.sh
 *
 *************************************************************************/

#define TIGHTDB_VERSION "$tigthdb_version_escaped"

#if $enable_replication
#  define TIGHTDB_ENABLE_REPLICATION 1
#endif

#define TIGHTDB_INSTALL_PREFIX      "$install_prefix_escaped"
#define TIGHTDB_INSTALL_EXEC_PREFIX "$install_exec_prefix_escaped"
#define TIGHTDB_INSTALL_INCLUDEDIR  "$install_includedir_escaped"
#define TIGHTDB_INSTALL_BINDIR      "$install_bindir_escaped"
#define TIGHTDB_INSTALL_LIBDIR      "$install_libdir_escaped"
#define TIGHTDB_INSTALL_LIBEXECDIR  "$install_libexecdir_escaped"
EOF
