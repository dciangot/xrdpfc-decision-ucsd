/* 
   Author: Matevz Tadel
 */

#include <set>
#include <vector>
#include <string>

#include "XrdPfcDecisionUcsd.hh"
#include "XrdSys/XrdSysError.hh"
#include "XrdCl/XrdClURL.hh"

using namespace XrdPfcDecisionUcsd;

extern "C"
{
XrdPfc::Decision *XrdFileCacheGetDecision(XrdSysError &log)
{
   return new Decision(log);
}
}

//==============================================================================

Decision::Decision(XrdSysError &log) : XrdPfc::Decision(), m_log(log)
{
   m_log.Say("XrdPfcDecisionUcsd::Decision instantiated.");
}

Decision::~Decision()
{
   m_log.Say("XrdPfcDecisionUcsd::Decision terminated.");
}

bool Decision::ConfigDecision(const char* params)
{
   pcrecpp::StringPiece input(params);
   
   pcrecpp::RE cent("([-+]?)([^ ]+)\\s*");

   std::string opt, re;

   while (cent.Consume(&input, &opt, &re))
   {
      if (opt.empty()) opt = "+";
      m_log.Say("  Decision ", opt[0] == '+' ? "Cache: " : "Do not cache: ", re.c_str());

      m_rules.push_back(Rule(re, opt[0] == '+'));
   }

   return true;
}

//==============================================================================

bool Decision::Decide(const std::string &lfn, XrdOss &) const
{
   m_log.Say("XrdPfcDecisionUcsd::Decision::Decide ", lfn.c_str());

   int nr = m_rules.size();
   for (int i = 0; i < nr; ++i)
   {
      const Rule &r = m_rules[i];

      m_log.Say("  trying ", r.m_re.pattern().c_str());

      if (r.m_re.PartialMatch(lfn))
      {
         m_log.Say("  match. returning ", r.m_pass ? "pass" : "no pass");
         return r.m_pass;
      }
   }

   return true;
}
