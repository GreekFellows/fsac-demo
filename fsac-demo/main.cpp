// add opengl32.lib!
// http://stackoverflow.com/questions/20223198/

// an always good guide to lnk2019
// http://stackoverflow.com/questions/12573816/

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <deque>
#include <typeindex>

#include "GL/glew.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "GLFW/glfw3.h"

#include "fsac/fsArea.h"
#include "fsac/fsChaosCalls.h"

GLFWwindow *onewindow;

void initFSAC();
void tickFSAC();
void renderScene();
void showImGui();

void showGeneInfoButton(const fsGeneInfo&, const int&, const std::string& = "");
void showAlleleInfoButton(const fsAlleleInfo&, const int&, const std::string& = "");
void showTrackDoublePlotterButton(void*, const int&, const std::string&, const std::string& = "");

void showGeneInfoContent(const fsGeneInfo&);		// do not call this manually
void showAlleleInfoContent(const fsAlleleInfo&);	// do not call this manually

void forGLFWErrorCallback(int, const char*);

int numAreas = 1;
int numPopulations = 1;

int areaCount = 0;
int blockCount = 0;
int populationCount = 0;
int doublePlotterCount = 0;

std::vector<fsArea*> areas;
std::vector<fsBlock*> blocks;
std::vector<fsPopulation*> populations;

bool keepSimulating = false, simulateNow = false;
double simulationIntervalMilliseconds = 10.0;
std::chrono::high_resolution_clock simulationClock;
std::chrono::high_resolution_clock::time_point lastSimulation;

struct doublePlotterInfo {
	int index;
	std::string name;
	std::vector<std::string> names;
	std::vector<void*> valuePointers;
	std::vector<std::type_index> valueTypes;
	std::vector<std::deque<double>> values;
	bool showWindow;

	doublePlotterInfo() : index(-1), name(""), showWindow(false) {

	}

	doublePlotterInfo(const int &cindex, const std::string &cname) : index(cindex), name(cname), showWindow(false) {

	}

	void track(const std::string &cname, void *pointer, const std::type_index &typeIndex) {
		names.push_back(cname);
		valuePointers.push_back(static_cast<double*>(pointer));
		valueTypes.push_back(typeIndex);
		values.push_back(std::deque<double>(1, *static_cast<typeIndex*>(pointer)));
		std::cout << values[0][0] << std::endl;
	}

	std::string makeWindowTitle() {
		return ("plotter " + std::to_string(index) + ": " + name);
	}
};

std::vector<doublePlotterInfo> doublePlotters;

void initFSAC() {
	lastSimulation = simulationClock.now();

	for (int ca = 0; ca != numAreas; ++ca) {
		fsArea *newArea = new fsArea(areaCount++);
		newArea->setSize(3, 3);

		for (int by = 0; by != newArea->height; ++by) {
			for (int bx = 0; bx != newArea->width; ++bx) {
				fsBlock *newBlock = newArea->addBlock(blockCount++);

				for (int np = 0; np != numPopulations; ++np) {
					fsPopulation *newPopulation = newBlock->addPopulation(populationCount++, std::to_string(rand() % 0xffffffff));

					// create genome
					newPopulation->addGeneById(fsGeneID::SPECIES_TYPE);
					switch (rand() % 3) {
					case 0:
						newPopulation->addAlleleById(fsAlleleID::ST_A);
						break;

					case 1:
						newPopulation->addAlleleById(fsAlleleID::ST_B);
						break;

					case 2:
						newPopulation->addAlleleById(fsAlleleID::ST_C);
						break;
					}

					newPopulation->addGeneById(fsGeneID::REPRODUCTION);
					newPopulation->addAlleleById(fsAlleleID::R_NORMAL);
					if (rand() % 2 == 0) {
						newPopulation->addAlleleById(fsAlleleID::R_PLUS);
					}
					if (rand() % 5 == 0) {
						newPopulation->addAlleleById(fsAlleleID::R_PPLUS);
					}

					newPopulation->addGeneById(fsGeneID::TOXIN_RELEASE);
					if (rand() % 10 == 0) {
						newPopulation->addAlleleById(fsAlleleID::TREL_A);
					}
					if (rand() % 10 == 0) {
						newPopulation->addAlleleById(fsAlleleID::TREL_B);
					}
					if (rand() % 10 == 0) {
						newPopulation->addAlleleById(fsAlleleID::TREL_C);
					}

					newPopulation->addGeneById(fsGeneID::TOXIN_RESISTANCE);
					if (rand() % 10 == 0) {
						newPopulation->addAlleleById(fsAlleleID::TRES_A);
					}
					if (rand() % 10 == 0) {
						newPopulation->addAlleleById(fsAlleleID::TRES_B);
					}
					if (rand() % 10 == 0) {
						newPopulation->addAlleleById(fsAlleleID::TRES_C);
					}

					populations.push_back(newPopulation);
				}

				blocks.push_back(newBlock);
			}
		}

		areas.push_back(newArea);
	}
}

void tickFSAC() {
	simulateNow = false;
	std::chrono::high_resolution_clock::time_point timeNow = simulationClock.now();

	if (keepSimulating) {
		long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - lastSimulation).count();
		if (elapsedTime >= simulationIntervalMilliseconds) {
			lastSimulation = timeNow;
			simulateNow = true;
		}
	}
	else {
		lastSimulation = timeNow;
	}

	if (simulateNow) {
		for (auto iter = areas.begin(); iter != areas.end(); ++iter) {
			(*iter)->simulate();
		}
	}
}

void renderScene() {

}

void showImGui() {
	ImGui::ShowTestWindow();

	ImGui::Begin("main controls");

	if (ImGui::TreeNode("observations")) {
		for (auto aiter = areas.begin(); aiter != areas.end(); ++aiter) {
			fsArea *currentArea = *aiter;

			if (ImGui::Button(("show area " + std::to_string(currentArea->index)).c_str())) {
				currentArea->showObservationWindow = true;
			}

			if (currentArea->showObservationWindow) {
				ImGui::Begin(currentArea->makeWindowTitle().c_str(), &currentArea->showObservationWindow);
				ImGui::Text("show block...");

				int newLineCount = 0;
				for (auto biter = currentArea->blocks.begin(); biter != currentArea->blocks.end(); ++biter, ++newLineCount) {
					fsBlock *currentBlock = *biter;

					if (ImGui::Button((std::to_string(currentBlock->index)).c_str(), ImVec2(20, 20))) {
						currentBlock->showObservationWindow = true;
					}

					if (currentBlock->showObservationWindow) {
						ImGui::Begin(currentBlock->makeWindowTitle().c_str(), &currentBlock->showObservationWindow);
						for (auto piter = currentBlock->populations.begin(); piter != currentBlock->populations.end(); ++piter) {
							fsPopulation *currentPopulation = *piter;

							if (ImGui::Button(("show population " + std::to_string(currentPopulation->index)).c_str())) {
								currentPopulation->showObservationWindow = true;
							}

							if (currentPopulation->showObservationWindow) {
								ImGui::Begin(currentPopulation->makeWindowTitle().c_str(), &currentPopulation->showObservationWindow);

								if (ImGui::TreeNode("size")) {
									ImGui::Text((std::to_string(currentPopulation->size)).c_str());

									void *ptr = &currentPopulation->size;
									std::cout << *static_cast<long long*>(ptr) << ", " << *static_cast<double*>(ptr) << std::endl;

									showTrackDoublePlotterButton(&currentPopulation->size, populationCount, "size");
									ImGui::TreePop();
								}

								if (ImGui::TreeNode("genome")) {
									int gCount = 0;
									for (auto giter = currentPopulation->genome.begin(); giter != currentPopulation->genome.end(); ++giter, ++gCount) {
										fsGene *currentGene = *giter;
										fsGeneInfo currentGeneInfo = fsGene::findGeneInfo(currentGene->id);

										ImGui::Text(currentGeneInfo.name.c_str());
										ImGui::SameLine();
										showGeneInfoButton(currentGeneInfo, gCount, "?");

										if (!currentGene->alleles.empty()) {
											int aCount = 0;
											for (auto aliter = currentGene->alleles.begin(); aliter != currentGene->alleles.end(); ++aliter, ++aCount) {
												fsAllele *currentAllele = *aliter;
												fsAlleleInfo currentAlleleInfo = fsAllele::findAlleleInfo(currentAllele->id);

												showAlleleInfoButton(currentAlleleInfo, aCount);
												ImGui::SameLine();
											}
										}
										else {
											ImGui::Text("(none)");
										}
										ImGui::Dummy(ImVec2(0, 0));
									}
									ImGui::TreePop();
								}

								ImGui::End();
							}
						}

						ImGui::End();
					}

					if (newLineCount % currentArea->width != (currentArea->width - 1)) {
						ImGui::SameLine();
					}
				}

				ImGui::End();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("simulation controls")) {
		if (ImGui::Button(keepSimulating ? "pause " : "resume " "simulation")) {
			keepSimulating = !keepSimulating;
		}

		float simulationIntervalMillisecondsFloat = simulationIntervalMilliseconds;
		ImGui::SliderFloat("simulation interval (ms)", &simulationIntervalMillisecondsFloat, 10, 30000);
		simulationIntervalMilliseconds = simulationIntervalMillisecondsFloat;

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("plotters")) {
		for (auto piter = doublePlotters.begin(); piter != doublePlotters.end(); ++piter) {
			doublePlotterInfo &currentDoublePlotterInfo = *piter;
			if (ImGui::Button(("show plotter " + std::to_string(currentDoublePlotterInfo.index) + ": " + currentDoublePlotterInfo.name).c_str())) {
				currentDoublePlotterInfo.showWindow = true;
			}

			if (currentDoublePlotterInfo.showWindow) {
				ImGui::Begin(currentDoublePlotterInfo.makeWindowTitle().c_str(), &currentDoublePlotterInfo.showWindow);

				for (int i = 0; i != currentDoublePlotterInfo.names.size(); ++i) {
					std::string &currentName = currentDoublePlotterInfo.names[i];
					double *currentValuePointer = static_cast<double*>(currentDoublePlotterInfo.valuePointers[i]);
					std::deque<double> &currentValues = currentDoublePlotterInfo.values[i];

					// http://stackoverflow.com/a/8619071/1517227
					std::vector<float> floats(currentValues.begin(), currentValues.end());

					// values from front to back in the deque are drawn from left (oldest) to right (newest)
					if (!currentValues.empty()) {
						ImGui::PlotLines(
							currentName.c_str(),
							&floats[0],	// http://stackoverflow.com/a/8682557/1517227
							(int)(currentValues.size())
							);
					}

					// update the values in the deque IF there was a simulation
					if (simulateNow) {
						if (currentValues.size() >= 50) {
							currentValues.pop_front();
						}
						currentValues.push_back(*currentValuePointer);
					}
				}

				ImGui::End();
			}
		}

		if (ImGui::Button("add plotter")) {
			doublePlotters.push_back(doublePlotterInfo(doublePlotterCount++, "an unnamed plotter"));
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void showGeneInfoButton(const fsGeneInfo &gi, const int &cid, const std::string &label) {
	bool buttonPressed = false;

	ImGui::PushID(cid);
	if (label.empty()) {
		buttonPressed = ImGui::Button((gi.name + " gene").c_str());
	} else {
		buttonPressed = ImGui::Button(label.c_str());
	}
	ImGui::PopID();

	if (!buttonPressed) {
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			showGeneInfoContent(gi);
			ImGui::EndTooltip();
		}
	}
	else {
		ImGui::OpenPopup(gi.makeWindowTitle().c_str());
	}

	if (ImGui::BeginPopup(gi.makeWindowTitle().c_str())) {
		showGeneInfoContent(gi);
		ImGui::EndPopup();
	}
}

void showAlleleInfoButton(const fsAlleleInfo &ai, const int &cid, const std::string &label) {
	bool buttonPressed = false;

	ImGui::PushID(cid);
	if (label.empty()) {
		buttonPressed = ImGui::Button(ai.name.c_str());
	}
	else {
		buttonPressed = ImGui::Button(label.c_str());
	}
	ImGui::PopID();

	if (!buttonPressed) {
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			showAlleleInfoContent(ai);
			ImGui::EndTooltip();
		}
	}
	else {
		ImGui::OpenPopup(ai.makeWindowTitle().c_str());
	}

	if (ImGui::BeginPopup(ai.makeWindowTitle().c_str())) {
		showAlleleInfoContent(ai);
		ImGui::EndPopup();
	}
}

void showGeneInfoContent(const fsGeneInfo &gi) {
	ImGui::Text(gi.makeWindowTitle().c_str());
	ImGui::Separator();
	ImGui::TextWrapped(gi.desc.c_str());
	ImGui::Separator();

	ImGui::Text("alleles: ");
	int aCount = 0;
	for (auto iter = gi.alleleIDs.begin(); iter != gi.alleleIDs.end(); ++iter, ++aCount) {
		showAlleleInfoButton(fsAllele::findAlleleInfo(*iter), aCount);
		ImGui::SameLine();
	}
	ImGui::Dummy(ImVec2(0, 0));
}

void showAlleleInfoContent(const fsAlleleInfo &ai) {
	ImGui::Text(ai.makeWindowTitle().c_str());
	ImGui::Separator();
	ImGui::TextWrapped(ai.desc.c_str());
	ImGui::Separator();

	ImGui::Text("gene: ");
	showGeneInfoButton(fsGene::findGeneInfo(ai.geneID), 0);
}

void showTrackDoublePlotterButton(void *pointer, const int &cid, const std::string &cname, const std::string &label) {
	bool buttonPressed = false;

	ImGui::PushID(cid);
	if (label.empty()) {
		buttonPressed = ImGui::Button("track in plotter");
	}
	else {
		buttonPressed = ImGui::Button(label.c_str());
	}
	ImGui::PopID();

	std::string popupTitle = "pop " + std::to_string((int)pointer);
	if (buttonPressed) {
		ImGui::OpenPopup(popupTitle.c_str());
	}

	if (ImGui::BeginPopup(popupTitle.c_str())) {
		for (auto piter = doublePlotters.begin(); piter != doublePlotters.end(); ++piter) {
			doublePlotterInfo &currentDoublePlotterInfo = *piter;

			if (ImGui::MenuItem(("to " + currentDoublePlotterInfo.makeWindowTitle()).c_str())) {
				currentDoublePlotterInfo.track(cname, pointer);
			}
		}

		ImGui::EndPopup();
	}
}

void forGLFWErrorCallback(int error, const char *desc) {
	std::cerr << "error: " << desc << std::endl;
}

int main(int argc, char** argv) {
	initFSAC();

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(forGLFWErrorCallback);

	onewindow = glfwCreateWindow(1000, 563, "fsac-demo", NULL, NULL);
	if (!onewindow) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(onewindow);

	ImGui_ImplGlfwGL3_Init(onewindow, true);

	// this guy saves the day
	// http://stackoverflow.com/questions/8302625/
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cout << glewGetErrorString(res);
		glfwTerminate();
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	while (!glfwWindowShouldClose(onewindow)) {
		tickFSAC();

		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		showImGui();

		int displayWidth, displayHeight;
		glfwGetFramebufferSize(onewindow, &displayWidth, &displayHeight);
		glViewport(0, 0, displayWidth, displayHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(0);
		renderScene();
		ImGui::Render();
		glfwSwapBuffers(onewindow);
	}

	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
	return 0;
}