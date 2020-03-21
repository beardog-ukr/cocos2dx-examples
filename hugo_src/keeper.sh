#!/usr/bin/env bash
#
# This script was created based on "bash-simple-plus" from:
# Bash Boilerplate: https://github.com/alphabetum/bash-boilerplate
# Copyright (c) 2015 William Melody • hi@williammelody.com

set -o nounset
set -o errexit
trap 'echo "Aborting due to errexit on line $LINENO. Exit code: $?" >&2' ERR
set -o errtrace
set -o pipefail
IFS=$'\n\t'

###############################################################################
# Environment
###############################################################################

# $_ME
# Set to the program's basename.
_ME=$(basename "${0}")

# Indicates there was an expected error in one of called functions.
_FUNC_ERROR_MSG=""

_CMD_TAG_CLEAR_PUBLIC="clear_public"
_CMD_TAG_PUBLISH="publish"
_CMD_TAG_SERVE="serve"

###############################################################################
# Debug
###############################################################################

# _debug()
#
# Usage:
#   _debug printf "Debug info. Variable: %s\n" "$0"
#
# A simple function for executing a specified command if the `$_USE_DEBUG`
# variable has been set. The command is expected to print a message and
# should typically be either `echo`, `printf`, or `cat`.
__DEBUG_COUNTER=0
_debug() {
  if [[ "${_USE_DEBUG:-"0"}" -eq 1 ]]
  then
    __DEBUG_COUNTER=$((__DEBUG_COUNTER+1))
    # Prefix debug message with "bug (U+1F41B)"
    printf "🐛  %s " "${__DEBUG_COUNTER}"
    "${@}"
    # printf "――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――\\n"
  fi
}
# debug()
#
# Usage:
#   debug "Debug info. Variable: $0"
#
# Print the specified message if the `$_USE_DEBUG` variable has been set.
#
# This is a shortcut for the _debug() function that simply echos the message.
debug() {
  _debug echo "${@}"
}

###############################################################################
# Die
###############################################################################

# _die()
#
# Usage:
#   _die printf "Error message. Variable: %s\n" "$0"
#
# A simple function for exiting with an error after executing the specified
# command. The command is expected to print a message and should typically
# be either `echo`, `printf`, or `cat`.
_die() {
  # Prefix die message with "cross mark (U+274C)", often displayed as a red x.
  printf "❌  "
  "${@}" 1>&2
  exit 1
}
# die()
#
# Usage:
#   die "Error message. Variable: $0"
#
# Exit with an error and print the specified message.
#
# This is a shortcut for the _die() function that simply echos the message.
die() {
  _die echo "${@}"
}

###############################################################################
# Help
###############################################################################

# _print_help()
#
# Usage:
#   _print_help
#
# Print the program help information.
_print_help() {
  cat <<HEREDOC
This script helps to perform various tasks required for managing the "Uncrustify formatting examples"

Usage:
  ${_ME} [--options] <command_name> [<example_name>] [<example_subname>]
  ${_ME} -h | --help

Options:
  -h --help    Display this help information.
  -v --vesion  (will be implemented later)

Commands:
  ${_CMD_TAG_CLEAR_PUBLIC}        Clears './public/' folder
  ${_CMD_TAG_PUBLISH}             Regenerates content and copies it from ./public to ../docs
  ${_CMD_TAG_SERVE}               Starts hugo localhost server

Examples:
  ${_ME} serve

HEREDOC
}

# Parse Options ###############################################################

# Initialize program option variables.
_PRINT_HELP=0
_USE_DEBUG=0

# Initialize additional expected option variables.
_OPTION_X=0
_SHORT_OPTION_WITH_PARAMETER=""
_LONG_OPTION_WITH_PARAMETER=""

_EXAMPLES_FOLDER=${PWD}

# _require_argument()
#
# Usage:
#   _require_argument <option> <argument>
#
# If <argument> is blank or another option, print an error message and  exit
# with status 1.
_require_argument() {
  # Set local variables from arguments.
  #
  # NOTE: 'local' is a non-POSIX bash feature and keeps the variable local to
  # the block of code, as defined by curly braces. It's easiest to just think
  # of them as local to a function.
  local _option="${1:-}"
  local _argument="${2:-}"

  if [[ -z "${_argument}" ]] || [[ "${_argument}" =~ ^- ]]
  then
    _die printf "Option requires an argument: %s\\n" "${_option}"
  fi
}

_COMMAND=""

while [[ ${#} -gt 0 ]]
do
  __option="${1:-}"
  __maybe_param="${2:-}"
  case "${__option}" in
    -h|--help)
      _PRINT_HELP=1
      ;;
    --debug)
      _USE_DEBUG=1
      ;;
    -x|--option-x)
      _OPTION_X=1
      ;;
    -o)
      _require_argument "${__option}" "${__maybe_param}"
      _SHORT_OPTION_WITH_PARAMETER="${__maybe_param}"
       shift
      ;;
    --endopts)
      # Terminate option parsing.
      break
      ;;
    -*)
      _die printf "Unexpected option: %s\\n" "${__option}"
      ;;
    *)
      # The first non-option argument is assumed to be the command name (as $_COMMAND).
      # Subsequent argument is example name (as _example_name).
      if [[ -n "${_COMMAND}" ]]
      then
        _die printf "Only one command is expected  (unexpected %s)\\n" "${__option}"
      else
        _COMMAND="${__option}"
      fi
      ;;
  esac
  shift
done

###############################################################################
# Program Functions (command processors)
###############################################################################

_serve() {
  hugo server -D
}

# #### #### #### #### #### #### #### #### #### #### #### #### #### ####

_clear_public() {
  echo "Will clear ./public folder"

  # Remove everything in ./public folder except the "dummy.txt" file
  find ${_pf} -mindepth 1 -type d | xargs rm -rf
  find ${_pf} -mindepth 1 -not -name "dummy.txt"  -exec rm -rf {} \;
}

# #### #### #### #### #### #### #### #### #### #### #### #### #### ####

_publish() {
  echo "Will publish ./public folder to ../docs"

  local _pf="${PWD}/public"
  if [ ! -d "${_pf}" ]; then
    _die printf "Tried to process folder %s and it does not exist\\n" "${_pf}"
  fi

  local _df="${PWD}/../docs"
  if [ ! -d "${_df}" ]; then
    _die printf "Expected to process docs folder %s and it does not exist\\n" "${_df}"
  fi

  # Remove everything in ./public folder except the "dummy.txt" file
  find ${_pf} -mindepth 1 -type d | xargs rm -rf
  find ${_pf} -mindepth 1 -not -name "dummy.txt"  -exec rm -rf {} \;

  # generate new files
  hugo

  # Remove everything in ../docs folder
  # note it should not get here is hugo fails
  rm -rf ${_df}/*

  # Copy files from ./public to ../docs
  cp -r ${_pf}/* ${_df}/

  printf "Done\\n"
}


###############################################################################
# Other functions
###############################################################################

# none

###############################################################################
# Main
###############################################################################

# _main()
#
# Usage:
#   _main [<options>] [<arguments>]
#
# Description:
#   Entry point for the program, handling basic option parsing and dispatching.
_main() {
  if ((_PRINT_HELP))
  then
    _print_help
  else
    _debug printf ">> Performing command ${_COMMAND}\\n"

    if [ "${_COMMAND}" == "${_CMD_TAG_CLEAR_PUBLIC}" ]; then
      _clear_public
      exit
    fi
    if [ "${_COMMAND}" == "${_CMD_TAG_PUBLISH}" ]; then
      _publish
      exit
    fi
    if [ "${_COMMAND}" == "${_CMD_TAG_SERVE}" ]; then
      _serve
      exit
    fi

    printf ">> Unknown command \"${_COMMAND}\"; nothing to do.\\n"
  fi
}

# Call `_main` after everything has been defined.
_main "$@"
