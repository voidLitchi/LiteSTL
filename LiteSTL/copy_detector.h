//A class used to detect copy, which could be used during testing
#pragma once
#include"general_define.h"

#include<iostream>

namespace NAME_GENERAL_NAMESPACE
{
	class CopyDetector
	{
	private:
		size_t uid;
		bool flag_shutup;

		static size_t uid_end;

	public:
		CopyDetector(bool _shutup = false)
			:flag_shutup(_shutup)
		{
			uid = uid_end++;
			if (!flag_shutup)
			{
				std::cout << "Detector No." << uid << ": constructed" << std::endl;
			}
		}
		CopyDetector(const CopyDetector& other)
		{
			flag_shutup = other.flag_shutup;
			uid = uid_end++;
			if (!flag_shutup)
			{
				std::cout << "Detector No." << uid << ": constructed by copy detector No." << other.uid << std::endl;
			}
		}
		CopyDetector(CopyDetector&& other)noexcept
		{
			flag_shutup = std::move(other.flag_shutup);
			uid = std::move(other.uid);
			if (!flag_shutup)
			{
				std::cout << "Detector No." << uid << ": moved." << std::endl;
			}
		}
		~CopyDetector()
		{
			if (!flag_shutup)
			{
				std::cout << "Detector No." << uid << ": destroyed." << std::endl;
			}
		}

		void swap(CopyDetector& other)
		{
			std::swap(flag_shutup, other.flag_shutup);
			std::swap(uid, other.uid);
		}

		CopyDetector& operator=(const CopyDetector& other)
		{
			if (this != &other)
			{
				CopyDetector temp(other);
				swap(temp);
			}
			return *this;
		}
		CopyDetector& operator=(CopyDetector&& other)noexcept
		{
			if (this != &other)
			{
				CopyDetector temp(std::move(other));
				swap(temp);
			}
			return *this;
		}

		void shutup()
		{
			flag_shutup = true;
		}
		static void reset_uid()
		{
			uid_end = 0;
		}

	};

	size_t CopyDetector::uid_end = 0;
}