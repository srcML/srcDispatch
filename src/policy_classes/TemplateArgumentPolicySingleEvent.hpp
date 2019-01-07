/**
 * @file TemplateArgumentPolicySingleEvent.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <srcSAXEventDispatchUtilities.hpp>

#include <exception>

#ifndef INCLUDED_TEMPLATE_ARGUMENT_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_TEMPLATE_ARGUMENT_POLICY_SINGLE_EVENT_HPP

class NamePolicy;
class TemplateArgumentPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:
    enum TemplateArgumentType { NAME, LITERAL, MODIFIER, POINTER, REFERENCE, RVALUE, OPERATOR, CALL };

    struct TemplateArgumentData {
        std::vector<std::pair<void *, TemplateArgumentType>> data;

        friend std::ostream & operator<<(std::ostream & out, const TemplateArgumentData & argumentData);

    };
    private:
        TemplateArgumentData data;
        std::size_t argumentDepth;
        NamePolicy * namePolicy;

    public:
        TemplateArgumentPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners);
        ~TemplateArgumentPolicy();
        virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override;
        virtual void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override;
    protected:
        virtual void * DataInner() const override;
    private:
        void InitializeTemplateArgumentPolicyHandlers();

        void CollectNamesHandler();
        void CollectOthersHandler();

};

#endif
