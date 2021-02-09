#ifndef XRDPFCDECISIONUCSD_H
#define XRDPFCDECISIONUCSD_H

#include "XrdPfc/XrdPfcDecision.hh"

#include <pcrecpp.h>

class XrdSysError;

namespace XrdPfcDecisionUcsd
{
/**
 *  @class XrdPfcDecisionUcsd::Decision 
 *    XRootD Proxy File Cache Decision plugin in use at UCSD T2.
 *    Use as:
 *        pfc.decisionlib libXrdPfcDecisionUcsd.so params
 *      each param is [+-]regexp
 *           + means do cachem, - means do not cache
 *           if no + or - is given, + is assumed.
 *    Regular expressions are processed in order they are specified.
 *    If no regexps match, "do cache" is assumed.
 *
 *  @Author: Matevz Tadel
 */

struct Rule
{
   pcrecpp::RE m_re;
   bool        m_pass;

   Rule(const std::string & re, bool pass) :
      m_re(re),
      m_pass(pass)
   {}
};


class Decision : public XrdPfc::Decision
{
public:

   Decision(XrdSysError &log);

   virtual ~Decision();

   virtual bool ConfigDecision(const char* params);

   virtual bool Decide(const std::string &, XrdOss &) const;

   XrdSysError &m_log;

protected:
   std::vector<Rule> m_rules;
};    

}

extern "C"
{
   XrdPfc::Decision *XrdFileCacheGetDecision(XrdSysError&);
}

#endif

