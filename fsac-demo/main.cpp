// add opengl32.lib!
// http://stackoverflow.com/questions/20223198/

// an always good guide to lnk2019
// http://stackoverflow.com/questions/12573816/

#include <iostream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "GLFW/glfw3.h"

#include "fsac/fsArea.h"

GLFWwindow *onewindow;

void initFSAC();
void tickFSAC();
void renderScene();
void showImGui();

void showGeneInfoButton(const fsGeneInfo&, const std::string& = "");
void showAlleleInfoButton(const fsAlleleInfo&, const std::string& = "");

void forGLFWErrorCallback(int, const char*);

int numAreas = 1;
int numPopulations = 1;

int areaCount = 0;
int blockCount = 0;
int populationCount = 0;

std::vector<fsArea*> areas;
std::vector<fsBlock*> blocks;
std::vector<fsPopulation*> populations;

void initFSAC() {
	for (int ca = 0; ca != numAreas; ++ca) {
		fsArea *newArea = new fsArea(areaCount++);
		newArea->setSize(3, 3);

		for (int by = 0; by != 3; ++by) {
			for (int bx = 0; bx != 3; ++bx) {
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
	for (auto iter = areas.begin(); iter != areas.end(); ++iter) {
		(*iter)->simulate();
	}
}

void renderScene() {

}

void showImGui() {
	ImGui::Begin("main controls");
	ImGui::Text("observations");

	for (auto aiter = areas.begin(); aiter != areas.end(); ++aiter) {
		fsArea *currentArea = *aiter;

		if (ImGui::Button(("show area " + std::to_string(currentArea->index)).c_str())) {
			currentArea->showObservationWindow = true;
		}

		if (currentArea->showObservationWindow) {
			ImGui::Begin(("area " + std::to_string(currentArea->index)).c_str(), &currentArea->showObservationWindow);
			ImGui::Text("show block...");

			int newLineCount = 0;
			for (auto biter = currentArea->blocks.begin(); biter != currentArea->blocks.end(); ++biter, ++newLineCount) {
				fsBlock *currentBlock = *biter;

				if (ImGui::Button((std::to_string(currentBlock->index)).c_str(), ImVec2(20, 20))) {
					currentBlock->showObservationWindow = true;
				}

				if (currentBlock->showObservationWindow) {
					ImGui::Begin(("block " + std::to_string(currentBlock->index)).c_str(), &currentBlock->showObservationWindow);
					for (auto piter = currentBlock->populations.begin(); piter != currentBlock->populations.end(); ++piter) {
						fsPopulation *currentPopulation = *piter;

						if (ImGui::Button(("show population " + std::to_string(currentPopulation->index)).c_str())) {
							currentPopulation->showObservationWindow = true;
						}

						if (currentPopulation->showObservationWindow) {
							ImGui::Begin(("population " + std::to_string(currentPopulation->index)).c_str(), &currentPopulation->showObservationWindow);

							if (ImGui::TreeNode("size")) {
								ImGui::Text((std::to_string(currentPopulation->size)).c_str());
								ImGui::TreePop();
							}

							if (ImGui::TreeNode("genome")) {
								for (auto giter = currentPopulation->genome.begin(); giter != currentPopulation->genome.end(); ++giter) {
									fsGene *currentGene = *giter;
									fsGeneInfo currentGeneInfo = fsGene::findGeneInfo(currentGene->id);

									ImGui::Text(currentGeneInfo.name.c_str());
									ImGui::SameLine();
									showGeneInfoButton(currentGeneInfo, "?");

									if (!currentGene->alleles.empty()) {
										for (auto aliter = currentGene->alleles.begin(); aliter != currentGene->alleles.end(); ++aliter) {
											fsAllele *currentAllele = *aliter;
											fsAlleleInfo currentAlleleInfo = fsAllele::findAlleleInfo(currentAllele->id);

											showAlleleInfoButton(currentAlleleInfo);
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

	ImGui::End();
}

void showGeneInfoButton(const fsGeneInfo &gi, const std::string &label) {
	if (label.empty()) {
		ImGui::Button((gi.name + " gene").c_str());
	} else {
		ImGui::Button(label.c_str());
	}

	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text((gi.name + " gene").c_str());
		ImGui::Separator();
		ImGui::Text(gi.desc.c_str());
		ImGui::Separator();

		ImGui::Text("alleles: ");
		for (auto iter = gi.alleleIDs.begin(); iter != gi.alleleIDs.end(); ++iter) {
			showAlleleInfoButton(fsAllele::findAlleleInfo(*iter));
		}

		ImGui::EndTooltip();
	}
}

void showAlleleInfoButton(const fsAlleleInfo &ai, const std::string &label) {
	if (label.empty()) {
		ImGui::Button(ai.name.c_str());
	}
	else {
		ImGui::Button(label.c_str());
	}

	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text((ai.name + " allele").c_str());
		ImGui::Separator();
		ImGui::Text(ai.desc.c_str());
		ImGui::Separator();

		ImGui::Text("gene: ");
		showGeneInfoButton(fsGene::findGeneInfo(ai.geneID));

		ImGui::EndTooltip();
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